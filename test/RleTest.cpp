/*
 * RleTest.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: nadav
 */

#include <thread>
#include <string>
#include "rle_interface.hpp"
#include "gtest/gtest.h"
#include "common/RleException.h"

namespace {

using namespace rle;
using std::string;
// The fixture for testing class Foo.
class RleTest : public ::testing::Test {
 protected:

  // Objects declared here can be used by all tests in the test case for Foo.
	std::string corePath = "/home/nadav/DQN/Arcade-Learning-Environment-2.0/snes9x2010/snes9x2010_libretro.so";
	std::string romPath = "/home/nadav/DQN/roms/mortal_kombat.sfc";
};

TEST_F(RleTest, simpleCtor) {
	rle::RLEInterface rle;
}

static void run_example(RLEInterface* rle, string romPath, string corePath){
	rle->loadROM(romPath, corePath);
	ActionVect legal_actions = rle->getMinimalActionSet();

	for (int episode=0; episode<1; ++episode) {
		int steps(0);
		while (!rle->game_over() && steps < 100) {
			Action a = legal_actions[rand() % legal_actions.size()];
			rle->act(a);
			++steps;
		}
		rle->reset_game();
	}
}

TEST_F(RleTest, multiThreading) {
	int numThreads(8);
	rle::RLEInterface rle[numThreads];
	std::vector<std::thread> threads;
	for(int i(0); i < numThreads; ++i){
		threads.push_back(std::thread(run_example, &rle[i], romPath, corePath));
	}
	for (auto& th : threads){
	    th.join();
	}
}

TEST_F(RleTest, runTwoAgentsNoDelete) {
	rle::RLEInterface rle;
	run_example(&rle, romPath, corePath);
	rle::RLEInterface rle2;
	rle.getEpisodeFrameNumber();
}

TEST_F(RleTest, runTwoAgentsNoDeleteWithDoubleInit) {
	rle::RLEInterface rle;
	run_example(&rle, romPath, corePath);
	rle::RLEInterface rle2;
	run_example(&rle2, romPath, corePath);
	rle.getEpisodeFrameNumber();
}

TEST_F(RleTest, runTwoAgentsWithDelete) {
	auto rle = new rle::RLEInterface();
	run_example(rle, romPath, corePath);
	delete rle;

	rle::RLEInterface rle2;
	run_example(&rle2, romPath, corePath);
}

TEST_F(RleTest, serialization) {
	rle::RLEInterface rle;
	run_example(&rle, romPath, corePath);
//	RLEState stateA = rle.cloneSystemState();
//	RLEState stateB = rle.cloneSystemState();
//	EXPECT_EQ(stateA, stateB);

	rle.saveState();
//	rle.loadState();

}


}  // namespace
