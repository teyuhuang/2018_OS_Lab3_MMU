FILENAME = test
MAIN = mmu
Include = utility.cpp
INPUT_BASE = ./lab3_assign/inputs/
exec: mmu
	@./$^ $(INPUT_BASE)in1
	@rm $^
mmu: $(MAIN).cpp $(Include)
	# @echo "=======IMPORTANT======"
	# @echo "Pleas use gcc 6.2.0 or above"
	# @echo "module load gcc-6.2.0"
	# @echo "======================"
	@g++ -std=c++0x $^ -o linker
clear: linker
	rm $^
loadModule:
	module load gcc-6.2.0
unloadModule:
	module unload gcc-6.2.0