FILENAME = test
MAIN = mmu
Include = utility.cpp Pager.cpp Essentials.cpp Simulator.cpp
INPUT_BASE = ./lab3_assign/inputs/
ARG = -oOPFSxfa -ac -f16
exec: mmu
	@./$^ $(ARG) $(INPUT_BASE)in1 $(INPUT_BASE)rfile
	@echo $(ARG)
	@rm $^
mmu: $(MAIN).cpp $(Include)
	@g++ -std=c++0x $^ -o $(MAIN)
out: mmu
	@echo "=======IMPORTANT======"
	@echo "Pleas use gcc 6.2.0 or above"
	@echo "module load gcc-6.2.0"
	@echo "======================"
clear: $(MAIN)
	rm $^
loadModule:
	module load gcc-6.2.0
unloadModule:
	# @echo "=======IMPORTANT======"
	# @echo "Pleas use gcc 6.2.0 or above"
	# @echo "module load gcc-6.2.0"
	# @echo "======================"
	module unload gcc-6.2.0