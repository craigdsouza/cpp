header files group related functionality into modules which are meant to be imported, not run by themselves.

namespaces help avoid naming conflicts.
using `using namespace std;` sparingly since it could again result in naming conflicts. It imports all the names into the global scope and this could get mixed up with names from other modules or locally defined names.
use more specific declarations like `using std::cout;` in whichever scope its needed.