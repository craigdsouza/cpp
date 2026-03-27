
std::cout outputs to the terminal (standard output)
std::ofstream outputs to file, e.g. txt

// if you want both terminal and txt file outputs
// declare an outfile
std::ofstream outfile("output.txt");
// declare a line
std::string line = "tile number " + std::to_string(tiles_int.size()) + tile + "\n";
std::cout << line;
outfile << line;

// if you want terminal output only
std::cout << "total number of tiles: " << tiles_int.size();

// later to view or save output to a txt file
./hello_map > output.txt  // writes to file
./hello_map >> output.txt //appends to file
./hello_map > output.txt 2>&1  // writes output and errors
./hello_map | tee output.txt // show in terminal and save to file simultaneously
