local M = {}

local uv = vim.uv or vim.loop

local function join(...)
  return vim.fs.joinpath(...)
end

local function read_json(path)
  local file, open_error = io.open(path, "r")
  if not file then
    return nil, open_error
  end

  local contents = file:read("*a")
  file:close()

  local ok, value = pcall(vim.json.decode, contents)
  if not ok then
    return nil, value
  end
  return value
end

local function resume(co, value)
  vim.schedule(function()
    local ok, error_message = coroutine.resume(co, value)
    if not ok then
      vim.notify(error_message, vim.log.levels.ERROR, { title = "CMake DAP" })
    end
  end)
end

local function run(args, cwd, callback)
  vim.system(args, { cwd = cwd, text = true }, function(result)
    vim.schedule(function()
      callback(result)
    end)
  end)
end

local function process_error(action, result)
  local output = result.stderr
  if not output or output == "" then
    output = result.stdout
  end
  output = output and vim.trim(output) or "unknown error"
  return string.format("%s failed with exit code %d:\n%s", action, result.code, output)
end

function M.project_root(bufnr)
  bufnr = bufnr or 0
  local filename = vim.api.nvim_buf_get_name(bufnr)
  local start = filename ~= "" and vim.fs.dirname(filename) or uv.cwd()

  local presets = vim.fs.find("CMakePresets.json", { path = start, upward = true })[1]
  if presets then
    return vim.fs.dirname(presets)
  end

  local lists = vim.fs.find("CMakeLists.txt", { path = start, upward = true })[1]
  return lists and vim.fs.dirname(lists) or nil
end

local function ensure_file_api_query(project_root)
  local query_directory =
    join(project_root, "build", "debug", ".cmake", "api", "v1", "query", "client-nvim-dap")
  vim.fn.mkdir(query_directory, "p")

  local query_path = join(query_directory, "codemodel-v2")
  local file, error_message = io.open(query_path, "w")
  if not file then
    return nil, error_message
  end
  file:close()
  return true
end

local function newest_index(reply_directory)
  local newest_path
  local newest_time = -1

  for name, item_type in vim.fs.dir(reply_directory) do
    if item_type == "file" and name:match("^index.*%.json$") then
      local path = join(reply_directory, name)
      local stat = uv.fs_stat(path)
      local modified = stat and stat.mtime and stat.mtime.sec or 0
      if modified > newest_time then
        newest_path = path
        newest_time = modified
      end
    end
  end

  return newest_path
end

local function executable_targets(project_root)
  local build_directory = join(project_root, "build", "debug")
  local reply_directory = join(build_directory, ".cmake", "api", "v1", "reply")
  local index_path = newest_index(reply_directory)
  if not index_path then
    return nil, "CMake File API did not produce a reply index"
  end

  local index, index_error = read_json(index_path)
  if not index then
    return nil, "Could not read CMake File API index: " .. tostring(index_error)
  end

  local client = index.reply and index.reply["client-nvim-dap"]
  local reference = client and client["codemodel-v2"]
  if not reference or not reference.jsonFile then
    return nil, "CMake File API codemodel reply is missing"
  end

  local codemodel, codemodel_error = read_json(join(reply_directory, reference.jsonFile))
  if not codemodel then
    return nil, "Could not read CMake codemodel: " .. tostring(codemodel_error)
  end

  local targets = {}
  for _, configuration in ipairs(codemodel.configurations or {}) do
    for _, target_reference in ipairs(configuration.targets or {}) do
      local target = read_json(join(reply_directory, target_reference.jsonFile))
      local artifact = target and target.artifacts and target.artifacts[1]
      if target and target.type == "EXECUTABLE" and artifact and artifact.path then
        local program = artifact.path
        if not vim.startswith(program, "/") and not program:match("^%a:[/\\]") then
          program = join(build_directory, program)
        end
        table.insert(targets, {
          name = target.name,
          program = vim.fs.normalize(program),
        })
      end
    end
  end

  table.sort(targets, function(left, right)
    return left.name < right.name
  end)
  if #targets == 0 then
    return nil, "No executable CMake targets were found"
  end
  return targets
end

function M.debug_program(bufnr)
  return coroutine.create(function(dap_run_co)
    local dap = require("dap")
    local project_root = M.project_root(bufnr)
    if not project_root then
      vim.notify("No CMake project found for the current buffer", vim.log.levels.ERROR)
      resume(dap_run_co, dap.ABORT)
      return
    end

    local query_ok, query_error = ensure_file_api_query(project_root)
    if not query_ok then
      vim.notify(query_error, vim.log.levels.ERROR, { title = "CMake DAP" })
      resume(dap_run_co, dap.ABORT)
      return
    end

    run({ "cmake", "--preset", "debug" }, project_root, function(configure_result)
      if configure_result.code ~= 0 then
        vim.notify(
          process_error("CMake configure", configure_result),
          vim.log.levels.ERROR,
          { title = "CMake DAP" }
        )
        resume(dap_run_co, dap.ABORT)
        return
      end

      local targets, target_error = executable_targets(project_root)
      if not targets then
        vim.notify(target_error, vim.log.levels.ERROR, { title = "CMake DAP" })
        resume(dap_run_co, dap.ABORT)
        return
      end

      vim.ui.select(targets, {
        prompt = "CMake executable target",
        format_item = function(target)
          return target.name
        end,
      }, function(target)
        if not target then
          resume(dap_run_co, dap.ABORT)
          return
        end

        run({
          "cmake",
          "--build",
          "--preset",
          "debug",
          "--target",
          target.name,
          "--parallel",
        }, project_root, function(build_result)
          if build_result.code ~= 0 then
            vim.notify(
              process_error("CMake build", build_result),
              vim.log.levels.ERROR,
              { title = "CMake DAP" }
            )
            resume(dap_run_co, dap.ABORT)
            return
          end
          resume(dap_run_co, target.program)
        end)
      end)
    end)
  end)
end

return M
