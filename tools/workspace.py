#!/usr/bin/env python3
"""Manage the independent CMake projects in this workspace."""

from __future__ import annotations

import argparse
import json
import re
import shutil
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PROJECTS_ROOT = ROOT / "projects"
TEMPLATE = ROOT / ".templates" / "sfml-project"
VSCODE_SETTINGS = ROOT / ".vscode" / "settings.json"
IDENTIFIER_PATTERN = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


def discover_projects() -> list[Path]:
    if not PROJECTS_ROOT.is_dir():
        return []

    projects = []
    for path in PROJECTS_ROOT.iterdir():
        if (
            path.is_dir()
            and not path.name.startswith(".")
            and (path / "CMakeLists.txt").is_file()
            and (path / "CMakePresets.json").is_file()
        ):
            projects.append(path)
    return sorted(projects, key=lambda item: item.name.casefold())


def load_json(path: Path) -> dict:
    if not path.exists():
        return {}

    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except json.JSONDecodeError as error:
        raise SystemExit(f"{path}: invalid JSON: {error}") from error

    if not isinstance(value, dict):
        raise SystemExit(f"{path}: expected a JSON object")
    return value


def write_json(path: Path, value: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        json.dumps(value, indent=2, ensure_ascii=False) + "\n",
        encoding="utf-8",
    )


def sync_vscode() -> list[Path]:
    projects = discover_projects()
    settings = load_json(VSCODE_SETTINGS)
    settings["cmake.sourceDirectory"] = [
        f"${{workspaceFolder}}/projects/{project.name}" for project in projects
    ]
    write_json(VSCODE_SETTINGS, settings)

    print("VS Code CMake projects:")
    if projects:
        for project in projects:
            print(f"  - {project.name}")
    else:
        print("  (none)")
    return projects


def default_identifier(directory_name: str) -> str:
    words = re.findall(r"[A-Za-z0-9]+", directory_name)
    identifier = "".join(word[:1].upper() + word[1:] for word in words)
    if not identifier:
        identifier = "SfmlProject"
    if identifier[0].isdigit():
        identifier = f"Chapter{identifier}"
    return identifier


def prompt_value(prompt: str, default: str | None = None) -> str:
    suffix = f" [{default}]" if default else ""
    value = input(f"{prompt}{suffix}: ").strip()
    return value or default or ""


def validate_directory_name(value: str) -> str:
    path = Path(value)
    if not value or path.name != value or value in {".", ".."}:
        raise SystemExit(
            "Project directory must be one immediate child name under projects/"
        )
    if value.startswith("."):
        raise SystemExit("Project directory must not be hidden")
    return value


def validate_identifier(label: str, value: str) -> str:
    if not IDENTIFIER_PATTERN.fullmatch(value):
        raise SystemExit(
            f"{label} must match {IDENTIFIER_PATTERN.pattern!r}; received {value!r}"
        )
    return value


def render_template(destination: Path, project_name: str, target_name: str) -> None:
    shutil.copytree(TEMPLATE, destination)
    replacements = {
        "__PROJECT_NAME__": project_name,
        "__TARGET_NAME__": target_name,
    }

    for path in destination.rglob("*"):
        if not path.is_file():
            continue
        text = path.read_text(encoding="utf-8")
        for old, new in replacements.items():
            text = text.replace(old, new)
        path.write_text(text, encoding="utf-8")


def create_project(args: argparse.Namespace) -> None:
    directory_name = args.directory or prompt_value("Project directory")
    directory_name = validate_directory_name(directory_name)

    suggested_identifier = default_identifier(directory_name)
    project_name = args.name or prompt_value("CMake project name", suggested_identifier)
    target_name = args.target or prompt_value("Executable target", project_name)
    project_name = validate_identifier("Project name", project_name)
    target_name = validate_identifier("Target name", target_name)

    PROJECTS_ROOT.mkdir(parents=True, exist_ok=True)
    destination = PROJECTS_ROOT / directory_name
    if destination.exists():
        raise SystemExit(f"Refusing to overwrite existing path: {destination}")
    if not TEMPLATE.is_dir():
        raise SystemExit(f"Project template is missing: {TEMPLATE}")

    try:
        render_template(destination, project_name, target_name)
    except Exception:
        if destination.exists():
            shutil.rmtree(destination)
        raise

    sync_vscode()
    print(f"Created projects/{directory_name} ({project_name}, target {target_name})")
    print(
        f"Configure with: cd projects/{directory_name} "
        "&& cmake --preset debug --fresh"
    )


def select_cleanup_projects(projects: list[Path]) -> list[Path]:
    if not projects:
        print("No CMake projects found")
        return []

    print("Select a project to clean:")
    for index, project in enumerate(projects, start=1):
        print(f"  {index}. {project.name}")
    print("  a. all projects")
    print("  q. cancel")

    while True:
        answer = input("Selection: ").strip().lower()
        if answer in {"q", "quit", "cancel", ""}:
            return []
        if answer in {"a", "all"}:
            return projects
        if answer.isdigit() and 1 <= int(answer) <= len(projects):
            return [projects[int(answer) - 1]]
        print("Enter a project number, 'a', or 'q'")


def confirm_cleanup(projects: list[Path]) -> bool:
    names = ", ".join(project.name for project in projects)
    answer = input(f"Remove generated files for {names}? [y/N]: ").strip().lower()
    return answer in {"y", "yes"}


def clean_project(project: Path) -> None:
    if project not in discover_projects():
        raise SystemExit(f"Refusing to clean unrecognized project: {project}")

    build_directory = project / "build"
    compile_database = project / "compile_commands.json"

    if build_directory.exists():
        shutil.rmtree(build_directory)
        print(f"Removed {build_directory.relative_to(ROOT)}")

    if compile_database.is_symlink() or compile_database.is_file():
        compile_database.unlink()
        print(f"Removed {compile_database.relative_to(ROOT)}")


def clean_projects(args: argparse.Namespace) -> None:
    projects = discover_projects()

    if args.all:
        selected = projects
    elif args.project:
        requested = (PROJECTS_ROOT / validate_directory_name(args.project)).resolve()
        selected = [project for project in projects if project.resolve() == requested]
        if not selected:
            raise SystemExit(f"Unknown CMake project: {args.project}")
    else:
        selected = select_cleanup_projects(projects)

    if not selected:
        print("Nothing cleaned")
        return
    if not args.yes and not confirm_cleanup(selected):
        print("Cleanup cancelled")
        return

    for project in selected:
        clean_project(project)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    new_parser = subparsers.add_parser("new", help="create an SFML project")
    new_parser.add_argument(
        "directory", nargs="?", help="new directory name under projects/"
    )
    new_parser.add_argument("--name", help="CMake project identifier")
    new_parser.add_argument("--target", help="executable target identifier")
    new_parser.set_defaults(handler=create_project)

    sync_parser = subparsers.add_parser(
        "sync", help="synchronize VS Code with discovered projects"
    )
    sync_parser.set_defaults(handler=lambda _args: sync_vscode())

    clean_parser = subparsers.add_parser("clean", help="remove generated project files")
    clean_parser.add_argument("project", nargs="?", help="project directory to clean")
    clean_parser.add_argument(
        "--all", action="store_true", help="clean every discovered project"
    )
    clean_parser.add_argument(
        "--yes", action="store_true", help="skip cleanup confirmation"
    )
    clean_parser.set_defaults(handler=clean_projects)

    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()
    if getattr(args, "all", False) and getattr(args, "project", None):
        parser.error("project and --all are mutually exclusive")
    args.handler(args)
    return 0


if __name__ == "__main__":
    sys.exit(main())
