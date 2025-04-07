#include "Input.h"

void Input::UpdateVars() {
    //Mouse
    {
        lmb_was_up = !LMBDOWN();
        lmb_was_down = LMBDOWN();
    }

    //Letters
    {
        c_was_up = !BUTTONDOWN(C_K);
        c_was_down = BUTTONDOWN(C_K);
        i_was_up = !BUTTONDOWN(I_K);
        i_was_down = BUTTONDOWN(I_K);
        j_was_up = !BUTTONDOWN(J_K);
        j_was_down = BUTTONDOWN(J_K);
        m_was_up = !BUTTONDOWN(M_K);
        m_was_down = BUTTONDOWN(M_K);
        o_was_up = !BUTTONDOWN(O_K);
        o_was_down = BUTTONDOWN(O_K);
    }
    //Special keys
    {
        tab_was_up = !TABDOWN();
        tab_was_down = TABDOWN();
    }
}

bool Input::KeyPressed(Keyboard::Key key) {
    switch (key) {
        case TAB:
            return tab_was_up and BUTTONDOWN(TAB);

        case O_K:
            return o_was_up and BUTTONDOWN(O_K);
    }
    return false;
}

bool Input::KeyPressed(Mouse::Button mb) {
    switch (mb) {
    case LMB:
        return lmb_was_up and BUTTONDOWN(LMB);

    case RMB:
        return rmb_was_up and BUTTONDOWN(RMB);

    case MID:
        return mid_was_up and BUTTONDOWN(MID);
    }
    return false;
}

bool Input::KeyReleased(Keyboard::Key key) {
    return false;
}

bool Input::KeyReleased(Mouse::Button mb) {
    switch (mb) {
    case LMB:
        return lmb_was_down and !BUTTONDOWN(LMB);

    case RMB:
        return rmb_was_down and !BUTTONDOWN(RMB);

    case MID:
        return mid_was_down and !BUTTONDOWN(MID);
    }
    return false;
}