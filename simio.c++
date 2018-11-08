#include "simio.h"

void Hash::write(const std::string&	filename) {
	std::ofstream out;
	if (exists(filename)) {
		if (!prompt("File exists, overwrite? (y|n) ")) {
			return;
		}
	}
	print("Writing file "+yellow+filename+res+"... ");
	out.open(filename);
	out << size << "\n" << minradius << "\n" << maxradius << "\n";
	nodeptr current = new node;
	for (int ii = 0; ii < size; ii ++) {
		current = table[ii];
		if (current != NULL) {
			int depth = current -> bodies.size();
			for (int kk = 0; kk < depth; kk ++) {
				if (current -> bodies.at(kk) != NULL) {
					out << current -> bodies.at(kk) -> writeFormat();
				}
				else {
					std::cout << red+white_back << " Error " << black_back << " body " << kk;
					std::cout << " in {bodies} at " << current << ". Exiting." << res << std::endl;
					return;
				}
			}
		}
	}
	println(green+"done"+res);
}

Hash* read(const std::string& filename) {
	if (!exists(filename)) {
		std::cout << red+white_back << " Error " << black_back << " file does not exist." << std::endl;
		return NULL;
	}
	print("Reading file "+yellow+filename+res+"... ");
	print("\n", 2);
	int gdebug = debug;
	if (debug < 3) {
		debug = 0;
	}
	std::ifstream file;
	std::string line;
	int n;
	file.open(filename);

	std::getline(file, line); 
	n++;
 	int nbodies = stof(line);
	Hash* hash = new Hash(nbodies);

	std::getline(file, line);
	n++;
	minradius = stof(line);

	std::getline(file, line);
	n++;
	maxradius = stof(line);
	
	blockwidth = (maxradius - minradius) / (nbodies - 1);
	println("  {minradius}:  "+magenta+bright+std::to_string(minradius)+res+"\n  {maxradius}:  "+
		bright+magenta+std::to_string(maxradius)+res+"\n  {blockwidth}: "+bright+magenta+
		std::to_string(blockwidth)+res, 3);
	println("Number of bodies: "+bright+red+std::to_string(nbodies)+res, 3);
	while (std::getline(file, line)) {
		n++;
		if (line == "Body") {
			double x;
			double y;
			double z;
			double radius;
			double mass;
			double speed;
			std::getline(file, line); 
			println(line, 3);
			n++;
			std::size_t start = line.find("(");
			if (start != std::string::npos) {
				x = stof(line.substr(start+1,line.find(",") - start-1));
				y = stof(line.substr(line.find(",")+2, line.find_last_of(",")-1));
				z = stof(line.substr(line.find_last_of(",")+2,line.find(")")-1));
			}
			else {
				println(red+white_back+" Error "+res+" File error at line "+std::to_string(n)+" in file "+yellow+filename+res);
				return NULL;
			}
			std::getline(file, line); 
			println(line, 3);
			n++;
			radius = stof(line.substr(line.find_last_of(" ")+1));
			std::getline(file, line); 
			println(line, 3);
			n++;
			mass = stof(line.substr(line.find_last_of(" ")+1));
			std::getline(file, line); 
			println(line, 3);
			n++;
			speed = stof(line.substr(line.find_last_of(" ")+1));
			CBody* body = new CBody(mass, radius, speed, x, y, z);
			hash -> addNode(body);
			print(body -> info(), 2);
		}
	}
	debug = gdebug;
	println(green+"done"+res);
	return hash;
}

std::string CBody::writeFormat(format f) {
	std::string formatted;
	switch(f) {
		case text:
			formatted.append("Body\n");
			formatted.append("\t Position ("+std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(z)+")\n");
			formatted.append("\t Radius   "+std::to_string(radius)+"\n");
			formatted.append("\t Mass     "+std::to_string(mass)+"\n");
			formatted.append("\t Speed    "+std::to_string(speed)+"\n\n");
			break;
	}
	print("--------- "+cyan+"Writestream "+res+"--------- \n"+formatted+"------------------------------- \n", 2);
	return formatted;
}

bool exists(const std::string& filename) {
	return std::experimental::filesystem::exists(filename);
}

bool prompt(const std::string& p) {
	char in;
	std::cout << std::flush;
	std::cout << p << std::flush;
	std::cin >> in;
	if (in == 'y') {
		return true;
	}
	else {
		return false;
	}
}

void print(const std::string& s, int depth) {
	if (debug >= depth && depth != -1) {
		std::cout << s << std::flush;
	}
}
void print(const int& i, int depth) {
	if (debug >= depth && depth != -1) {
		std::cout << i << std::flush;
	}
}
void print(const double& d, int depth) {
	if (debug >= depth && depth != -1) {
		std::cout << d << std::flush;
	}
}
void print(const float& f, int depth) {
	if (debug >= depth && depth != -1) {
		std::cout << f << std::flush;
	}
}
void println(const std::string& s, int depth) {
	if (debug >= depth && depth != -1) {
		print(s, depth);
		std::cout << std::endl;
	}
}
void println(const double& d, int depth) {
	if (debug >= depth && depth != -1) {
		print(d, depth);
		std::cout << std::endl;
	}
}
void println(const float& f, int depth) {
	if (debug >= depth && depth != -1) {
		print(f, depth);
		std::cout << std::endl;
	}
}
void println(const int& i, int depth) {
	if (debug >= depth && depth != -1) {
		print(i, depth);
		std::cout << std::endl;
	}
}
