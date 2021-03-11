default:
	g++ -std=c++17 -Ijson/include -o plotVars PlotTrainVars.cpp `root-config --cflags --glibs`

