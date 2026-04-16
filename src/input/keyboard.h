#ifndef KEYBOARD_H
#define KEYBOARD_H

/**
 * @brief Cross-platform keyboard input without Enter key
 */

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

class Keyboard {
public:
    /**
     * @brief Get a single character without waiting for Enter
     * @return Character code
     */
    static char getKey() {
#ifdef _WIN32
        return _getch();
#else
        static struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);  // Non-canonical, no echo
        newt.c_cc[VMIN] = 1;
        newt.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        char c;
        read(STDIN_FILENO, &c, 1);
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return c;
#endif
    }
    
    /**
     * @brief Check if key is available without blocking
     * @return true if key available
     */
    static bool keyAvailable() {
#ifdef _WIN32
        return _kbhit();
#else
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        newt.c_cc[VMIN] = 0;
        newt.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        char c;
        int result = read(STDIN_FILENO, &c, 1);
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return (result > 0);
#endif
    }
};

#endif // KEYBOARD_H
