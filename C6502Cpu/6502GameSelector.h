//
// Copyright (c) 2016, Paul R. Swan
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include <C6502GenericGame.h>
#include <CTomahawk777Game.h>
#include <CAsteroidsGame.h>
#include <CAsteroidsDeluxeGame.h>
#include <CLunarLanderGame.h>
#include <CBattlezoneGame.h>
//
// The initial selector to select the game to test.
//
static const SELECTOR s_gameSelector[] PROGMEM = {//0123456789abcde
    {"Generic 2716   ",  onSelectGeneric, (void*) (C6502GenericGame::createInstance2716),            false},
    {"Generic 2732   ",  onSelectGeneric, (void*) (C6502GenericGame::createInstance2732),            false},
    {"Asteroids (4)  ",  onSelectGame,    (void*) (CAsteroidsGame::createInstanceSet4),              false},
//    {"Asteroids(4)clk",  onSelectGame,    (void*) (CAsteroidsGame::createInstanceClockMasterSet4),   false},
    {"Asteroids (2)  ",  onSelectGame,    (void*) (CAsteroidsGame::createInstanceSet2),              false},
//    {"Asteroids(2)clk",  onSelectGame,    (void*) (CAsteroidsGame::createInstanceClockMasterSet2),   false},
    {"Asteroids (1)  ",  onSelectGame,    (void*) (CAsteroidsGame::createInstanceSet1),              false},
//    {"Asteroids(1)clk",  onSelectGame,    (void*) (CAsteroidsGame::createInstanceClockMasterSet1),   false},
    {"AstDlx (3)     ",  onSelectGame,    (void*) (CAsteroidsDeluxeGame::createInstanceSet3),        false},
//    {"AstDlx (3) clk ",  onSelectGame,    (void*) (CAsteroidsDeluxeGame::createInstanceClockMasterSet3),   false},
    {"AstDlx (2)     ",  onSelectGame,    (void*) (CAsteroidsDeluxeGame::createInstanceSet2),              false},
//    {"AstDlx (2) clk ",  onSelectGame,    (void*) (CAsteroidsDeluxeGame::createInstanceClockMasterSet2),   false},
    {"AstDlx (1)     ",  onSelectGame,    (void*) (CAsteroidsDeluxeGame::createInstanceSet1),              false},
//    {"AstDlx (1) clk ",  onSelectGame,    (void*) (CAsteroidsDeluxeGame::createInstanceClockMasterSet1),   false},
    {"BZone (2)      ",  onSelectGame,    (void*) (CBattlezoneGame::createInstanceSet2),             false},
//    {"BZone (2) clk  ",  onSelectGame,    (void*) (CBattlezoneGame::createInstanceClockMasterSet2),  false},
    {"BZone (1)      ",  onSelectGame,    (void*) (CBattlezoneGame::createInstanceSet1),             false},
//    {"BZone (1) clk  ",  onSelectGame,    (void*) (CBattlezoneGame::createInstanceClockMasterSet1),  false},
    {"LLander (2)    ",  onSelectGame,    (void*) (CLunarLanderGame::createInstanceSet2),            false},
//    {"LLander (2) clk",  onSelectGame,    (void*) (CLunarLanderGame::createInstanceClockMasterSet2), false},
    {"LLander (1)    ",  onSelectGame,    (void*) (CLunarLanderGame::createInstanceSet1),            false},
//    {"LLander (1) clk",  onSelectGame,    (void*) (CLunarLanderGame::createInstanceClockMasterSet1), false},
    {"T-777   (5)    ",  onSelectGame,    (void*) (CTomahawk777Game::createInstanceSet5),            false},
    {"T-777   (5) clk",  onSelectGame,    (void*) (CTomahawk777Game::createInstanceClockMasterSet5), false},
    { 0, 0 }
};


