import os

from pytest_check import check


@check.check_func
def is_dir(path: str):
    assert os.path.isdir(path)


@check.check_func
def is_file(path: str):
    assert os.path.isfile(path)


def test_files_in_place():
    os.chdir(os.environ["VELOCITAS_APP_ROOT"])

    # devcontainer
    is_dir(".devcontainer")
    is_dir(".devcontainer/scripts")
    is_file(".devcontainer/scripts/configure-proxies.sh")
    is_file(".devcontainer/scripts/container-set.sh")
    is_file(".devcontainer/scripts/onCreateCommand.sh")
    is_file(".devcontainer/scripts/postStartCommand.sh")
    is_file(".devcontainer/scripts/reinstall-cmake.sh")
    is_file(".devcontainer/devcontainer.json")
    is_file(".devcontainer/Dockerfile")

    # github integration
    is_dir(".github")
    is_dir(".github/ISSUE_TEMPLATE")
    is_file(".github/ISSUE_TEMPLATE/bug-report.yml")
    is_file(".github/ISSUE_TEMPLATE/feature-request.yml")
    is_file(".github/ISSUE_TEMPLATE/question.yml")

    is_dir(".github/scripts")
    is_file(".github/scripts/deploy_image_from_artifact.sh")
    is_file(".github/scripts/junit.tpl")

    is_dir(".github/workflows")
    is_file(".github/workflows/build-docker-image.yml")
    is_file(".github/workflows/build-multiarch-image.yml")
    is_file(".github/workflows/check-devcontainer.yml")
    is_file(".github/workflows/check-licenses.yml")
    is_file(".github/workflows/ci.yml")
    is_file(".github/workflows/ensure-lifecycle.yml")
    is_file(".github/workflows/gen-desired-state.yml")
    is_file(".github/workflows/release.yml")
    is_file(".github/PULL_REQUEST_TEMPLATE.md")

    # vscode project
    is_dir(".vscode")
    is_file(".vscode/launch.json")
    is_file(".vscode/tasks.json")
    is_file(".vscode/c_cpp_properties.json")

    # app
    is_dir("app")
    is_file("app/src/CMakeLists.txt")
    is_file("app/src/Launcher.cpp")
    is_dir("app/tests")
    is_dir("app/tests/utests")
    is_file("app/tests/CMakeLists.txt")
    is_file("app/tests/utests/CMakeLists.txt")
    is_file("app/AppManifest.json")
    is_file("app/Dockerfile")
    is_file("app/CMakeLists.txt")

    # conan profiles
    is_dir(".conan")
    is_dir(".conan/profiles")
    is_file(".conan/profiles/linux_aarch64_debug")
    is_file(".conan/profiles/linux_aarch64_release")
    is_file(".conan/profiles/linux_x86_64_debug")
    is_file(".conan/profiles/linux_x86_64_release")

    # general things
    is_file(".clang-format")
    is_file(".clang-tidy")
    is_file(".dockerignore")
    is_file(".gitattributes")
    is_file(".gitignore")
    is_file(".gitmodules")
    is_file(".licensechecker.yml")
    is_file(".pre-commit-config.yaml")
    is_file(".velocitas.json")
    is_file("CMakeLists.txt")
    is_file("CPPLINT.cfg")
    is_file("LICENSE")
    is_file("NOTICE-3RD-PARTY-CONTENT.md")
    is_file("NOTICE.md")
    is_file("build.sh")
    is_file("conanfile.txt")
    is_file("gcovr.cfg")
    is_file("install_dependencies.sh")
    is_file("README.md")
    is_file("requirements.txt")
    is_file("whitelisted-licenses.txt")
