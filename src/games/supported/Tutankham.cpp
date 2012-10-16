/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2012 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "Tutankham.hpp"

#include "../RomUtils.hpp"


TutankhamSettings::TutankhamSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* TutankhamSettings::clone() const { 
    
    RomSettings* rval = new TutankhamSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void TutankhamSettings::step(const System& system) {

    // update the reward
    int score = getDecimalScore(0x9C, 0x9A, &system);
    int reward = score - m_score;
    m_reward = reward;
    m_score = score;

    // update terminal status
    int lives = readRam(&system, 0x9E);
    // byte 0x81 is set to 0x84 when the game is loaded, but not reset
    int some_byte = readRam(&system, 0x81);

    m_terminal = lives == 0 && some_byte != 0x84;
}


/* is end of game */
bool TutankhamSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t TutankhamSettings::getReward() const { 

    return m_reward; 
}


/* is an action part of the minimal set? */
bool TutankhamSettings::isMinimal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_LEFT:
        case PLAYER_A_RIGHT:
        case PLAYER_A_DOWN:
        case PLAYER_A_UP:
        case PLAYER_A_FIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_RIGHTFIRE:
        case PLAYER_A_DOWNFIRE:
        case PLAYER_A_UPFIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void TutankhamSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}
        
/* saves the state of the rom settings */
void TutankhamSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void TutankhamSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
}

