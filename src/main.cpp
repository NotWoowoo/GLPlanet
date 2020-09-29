#include "runtimeStages.h"

int main(int argc, char** argv) {
	RuntimeStages::init();
	RuntimeStages::run();
	RuntimeStages::end();
	
	return 0;
}