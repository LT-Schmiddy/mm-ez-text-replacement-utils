import pathlib, subprocess, os, shutil, tomllib, zipfile
import build_n64recomp_tools as bnt

USING_ASSETS_ARCHIVE = True

project_root = pathlib.Path(__file__).parent

mod_data = tomllib.loads(project_root.joinpath("mod.toml").read_text())
tests_data = tomllib.loads(project_root.joinpath("tests.toml").read_text())

build_mod_dir = project_root.joinpath(f"build/mod")
build_mod_nrm_file = build_mod_dir.joinpath(f"{mod_data['inputs']['mod_filename']}.nrm")

build_tests_dir = project_root.joinpath(f"build/tests")
build_tests_nrm_file = build_tests_dir.joinpath(f"{tests_data['inputs']['mod_filename']}.nrm")

runtime_mods_dir = project_root.joinpath("runtime/mods")
runtime_mod_nrm_file = runtime_mods_dir.joinpath(f"{mod_data['inputs']['mod_filename']}.nrm")
runtime_tests_nrm_file = runtime_mods_dir.joinpath(f"{tests_data['inputs']['mod_filename']}.nrm")

assets_archive_path = project_root.joinpath("assets_archive.zip")
assets_extract_path = project_root.joinpath("assets_extracted")


def run_build():
    # Unzipping Archive:
    if USING_ASSETS_ARCHIVE and not assets_extract_path.is_dir():
        print(f"Assets folder '{assets_extract_path.name}' not found. Extracting assets from '{assets_archive_path.name}'...")
        with zipfile.ZipFile(assets_archive_path, 'r') as zip_ref:
            zip_ref.extractall(assets_extract_path)
            
    
    if not bnt.build_dir.exists():
        print("N64Recomp tools not built. Building now...")
        bnt.rebuild_tools();

    # Build Mods:
    mod_make_run = subprocess.run(
        [
            bnt.deps["make"],
        ],
        cwd=pathlib.Path(__file__).parent
    )
    if mod_make_run.returncode != 0:
        raise RuntimeError("Make failed to build mod binaries.")

    mod_RecompModTool_run = subprocess.run(
        [
            bnt.get_RecompModTool_path(),
            "mod.toml",
            "build/mod"
        ],
        cwd=pathlib.Path(__file__).parent
    )
    if mod_RecompModTool_run.returncode != 0:
        raise RuntimeError("RecompModTool failed to build mod.")

    # Build Tests:
    tests_make_run = subprocess.run(
        [
            bnt.deps["make"],
            "BUILD_MODE=TESTS"
        ],
        cwd=pathlib.Path(__file__).parent
    )
    if tests_make_run.returncode != 0:
        raise RuntimeError("Make failed to build test binaries.")

    tests_RecompModTool_run = subprocess.run(
        [
            bnt.get_RecompModTool_path(),
            "tests.toml",
            "build/tests"
        ],
        cwd=pathlib.Path(__file__).parent
    )
    if tests_RecompModTool_run.returncode != 0:
        raise RuntimeError("RecompModTool failed to build tests.")
    
    
    # Copying files for debugging:
    os.makedirs(runtime_mods_dir, exist_ok=True)
    shutil.copy(build_mod_nrm_file, runtime_mod_nrm_file)
    shutil.copy(build_tests_nrm_file, runtime_tests_nrm_file)

if __name__ == '__main__':
    run_build()