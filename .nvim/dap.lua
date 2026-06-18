local source = debug.getinfo(1, "S").source:sub(2)
local nvim_directory = vim.fs.dirname(source)
local workspace_root = vim.fs.dirname(nvim_directory)
local cmake = dofile(vim.fs.joinpath(nvim_directory, "cmake.lua"))
local dap = require("dap")

local function lift_platform_settings(config)
  local platform
  if vim.fn.has("win32") == 1 then
    platform = "windows"
  elseif vim.fn.has("mac") == 1 then
    platform = "osx"
  else
    platform = "linux"
  end

  local platform_settings = config[platform]
  config.windows = nil
  config.osx = nil
  config.linux = nil
  if platform_settings then
    config = vim.tbl_extend("force", config, platform_settings)
  end
  return config
end

local function read_launch_configurations()
  local path = vim.fs.joinpath(workspace_root, ".vscode", "launch.json")
  local file = io.open(path, "r")
  if not file then
    return {}
  end

  local contents = file:read("*a")
  file:close()
  local ok, launch = pcall(vim.json.decode, contents)
  if not ok or type(launch) ~= "table" then
    vim.notify("Could not parse " .. path, vim.log.levels.ERROR, { title = "CMake DAP" })
    return {}
  end
  return launch.configurations or {}
end

dap.providers.configs["dap.launch.json"] = function(bufnr)
  local configurations = {}

  for _, source_config in ipairs(read_launch_configurations()) do
    if source_config.type == "lldb" then
      local config = lift_platform_settings(vim.deepcopy(source_config))
      config.type = "codelldb"
      config.preLaunchTask = nil
      config.program = function()
        return cmake.debug_program(bufnr)
      end
      config.cwd = cmake.project_root(bufnr) or workspace_root
      table.insert(configurations, config)
    end
  end

  return configurations
end

return {
  configurations = {},
}
