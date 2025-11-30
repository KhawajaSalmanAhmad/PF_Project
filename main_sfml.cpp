
#include "app.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <level_file.lvl>" << endl;
        cout << "Example: " << argv[0] << " levels/easy_level.lvl" << endl;
        return 1;
    }
    
    cout << "=== Switchback Rails - SFML Version ===" << endl;
    cout << "Controls:" << endl;
    cout << "  SPACE    - Pause/Resume" << endl;
    cout << "  .        - Step one tick" << endl;
    cout << "  UP/DOWN  - Adjust speed" << endl;
    cout << "  ESC      - Exit" << endl;
    cout << endl;
    
    if (!initApp(argv[1])) {
        cerr << "Failed to initialize application!" << endl;
        return 1;
    }
    
    runApp();
    cleanupApp();
    
    cout << "Simulation complete!" << endl;
    
    return 0;
}
