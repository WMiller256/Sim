#include "csim.h"
#include "simio.h"
#include "global.h"

Pos::Pos() : vec() {
}
Pos::Pos(double X, double Y, double Z) : vec(X, Y, Z) {
}

double Pos::originDist() {
	double dist = sqrt(pow(X(),2) + pow(Y(),2) + pow(Z(),2));
//	printrln(in("Pos", "originDist")+"  "+info(), scientific(dist), 4);//	printrln(in("Pos", "originDist")+"  "+info(), scientific(dist), 4);
	if (dist < minradius) {
		minradius = dist;
		blockwidth = (maxradius - minradius) / (nbodies - 1);
//		println("                         {maxradius}:  "+bright+magenta+std::to_string(maxradius)+res+//			    "\n                         {minradius}:  "+bright+magenta+std::to_string(minradius)+res+//			    "\n                         {nbodies}:    "+bright+magenta+std::to_string(nbodies)+res+//			    "\n                         {blockwidth}: "+bright+magenta+std::to_string(blockwidth)+res, 5);//			    "\n                         {blockwidth}: "+bright+magenta+std::to_string(blockwidth)+res, 5);
		if (blockwidth <= 0) {
			blockwidth = 1.0;
		}
	}
	if (dist > maxradius) {
		maxradius = dist;
		blockwidth = (maxradius - minradius) / (nbodies - 1);
		blockwidth = (maxradius - minradius) / (nbodies - 1);
//		println("                         {maxradius}:  "+bright+magenta+std::to_string(maxradius)+res+//			    "\n                         {minradius}:  "+bright+magenta+std::to_string(minradius)+res+//			    "\n                         {nbodies}:    "+bright+magenta+std::to_string(nbodies)+res+//			    "\n                         {blockwidth}: "+bright+magenta+std::to_string(blockwidth)+res, 5);//			    "\n                         {blockwidth}: "+bright+magenta+std::to_string(blockwidth)+res, 5);
		if (blockwidth <= 0) {
			blockwidth = 1.0;
		}
	}

	return dist;
}
vec Pos::direction(Pos target) {
	vec v(target.X() - X(), target.Y() - Y(), target.Z() - Z());
	double m = magnitude(v);
	v = v / m;
//	printrln(in("Pos","direction")+" {"+green+"target"+res+"} - "+target.info(2)+" to {"+green+"this"+res+"} - "+info(2), v.info(2), 4);//	printrln(in("Pos","direction")+" {"+green+"target"+res+"} - "+target.info(2)+" to {"+green+"this"+res+"} - "+info(2), v.info(2), 4);
	return v;
}
