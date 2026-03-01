import subprocess
import sys
import platform
from abc import ABC, abstractmethod
from pathlib import Path
from typing import List

# 1. The Contract (Abstract Base Class)
class CompilationStrategy(ABC):
    @abstractmethod
    def get_libraries(self) -> List[str]:
        pass

    @abstractmethod
    def get_output_name(self) -> str:
        pass

# 2. Concrete Implementation for Windows
class WindowsStrategy(CompilationStrategy):
    def get_libraries(self) -> List[str]:
        return ["-lwsock32", "-lws2_32"]

    def get_output_name(self) -> str:
        return "server.exe"

# 3. Concrete Implementation for Linux
class LinuxStrategy(CompilationStrategy):
    def get_libraries(self) -> List[str]:
        # Linux usually uses pthread for networking/threading
        return ["-lpthread"]

    def get_output_name(self) -> str:
        return "server"

# 4. The Orchestrator (Uses the contract, not concrete values)
class ProjectBuilder:
    def __init__(self, strategy: CompilationStrategy):
        self._strategy = strategy

    def build(self, src_dir: Path):
        source_files = [str(p) for p in src_dir.rglob("*.c")]
        
        if not source_files:
            print(f"No .c files found in {src_dir}.")
            return

        command = (
            ["gcc"] + 
            source_files + 
            ["-o", self._strategy.get_output_name()] + 
            self._strategy.get_libraries()
        )

        try:
            print(f"Running: {' '.join(command)}")
            subprocess.run(command, check=True)
            print("Build successful.")
        except subprocess.CalledProcessError as e:
            print(f"Build failed: {e}")
            sys.exit(1)

# 5. Factory Logic to select the strategy based on OS
def get_strategy() -> CompilationStrategy:
    current_os = platform.system()
    if current_os == "Windows":
        return WindowsStrategy()
    elif current_os == "Linux":
        return LinuxStrategy()
    else:
        raise NotImplementedError(f"OS '{current_os}' is not supported.")

if __name__ == "__main__":
    try:
        strategy = get_strategy()
        builder = ProjectBuilder(strategy)
        builder.build(Path("src"))
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
