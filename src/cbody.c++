#include "cbody.h"
#include "simio.h"
#include "global.h"

CBody::CBody() {
	init();
}
CBody::CBody(double Mass, double Radius, double Speed) {
	init();
	mass = Mass;
	radius = Radius;
	speed = Speed;
	xv = Speed;
	print(info(), 2);
}
CBody::CBody(double Mass, double Radius, double Speed, double X, double Y, double Z, int H) {
	init();
	x = X;
	y = Y;
	z = Z;
	h = H;
	pos = Pos(x, y, z);
	mass = Mass;
	radius = Radius;
	speed = Speed;
	xv = Speed;
	print(info(), 2);
}
CBody::CBody(double Mass, double Radius, double Speed, Pos p) {
	init();
	x = p.x;
	y = p.y;
	z = p.z;
	pos = p;
	mass = Mass;
	radius = Radius;
	speed = Speed;
	xv = Speed;
	print(info(), 2);
}
CBody::~CBody() {
	// Destructor
}

std::string CBody::Name() {
	return name;
}
void CBody::Name(std::string newName) {
	name = newName;
}
void CBody::setParent(CBody* Parent) {
	parent = Parent;
}
CBody* CBody::getParent() {
	return parent;
}
void CBody::Type(bodyType t) {
	type = t;
}
bodyType CBody::Type() {
	return type;
}

double CBody::Mass() {
	return mass;
}
double CBody::Radius() {
	return radius;
}
double CBody::Speed() {
	return magnitude(Vel(xv, yv, zv));
}
vec CBody::Velocity(vec v) {
	xv = v.x;
	yv = v.y;
	zv = v.z;
	return vec(xv, yv, zv);
}
Vel CBody::Velocity() {
	return Vel(xv, yv, zv);
}
vec CBody::accelerate(vec dv) {		// Accelerate the body by the given vector
	xv += dv.x;
	yv += dv.y;
	zv += dv.z;
	return vec(xv, yv, zv);
}
void CBody::Position(vec v) {
	x = v.x;
	y = v.y;
	z = v.z;
	pos = v;
}

double CBody::originDist() {
	double dist = pos.originDist();
	printrln(in("CBody", "originDist")+pos.info(), scientific(dist), 4);
	return dist;
}
Pos CBody::COM(CBody* target) {
	if (this == target) {
		error("{target} and {this} are the same, center of mass will be {this -> pos}.", __LINE__, __FILE__);
	}
	double m1 = Mass();
	double m2 = target -> Mass();
	double M = m1 + m2;
	if (M > 0.0) {
		return Pos((target -> pos * m1 + pos * m2) / M);		// Surprisingly, returning a (Pos) object instead of a pointer is
	}															// 1000x faster in this case, due to the cost of pointer construction
	else if (M == 0.0) {
		warning("Sum of masses is zero - {"+green+"m1"+res+"}: "+scientific(m1)+" {"+green+"m2"+res+"}. Returning (0,0,0).", __LINE__, __FILE__);
		return Pos();
	}
	else {
		error("Sum of masses is negative - {"+green+"m1"+res+"}: "+scientific(m1)+" {"+green+"m2"+res+"}. Returning (0,0,0).", __LINE__, __FILE__);
		return Pos();
	}
}
Pos CBody::COM(CBody target) {
	if (*this == target) {
		error("{target} and {this} are the same, center of mass will be {this -> pos}.", __LINE__, __FILE__);
	}
	double m1 = Mass();
	double m2 = target.Mass();
	double M = m1 + m2;
	if (M > 0.0) {
		return Pos((target.pos * m1 + pos * m2) / M);			// Surprisingly, returning a (Pos) object instead of a pointer is
	}															// 1000x faster in this case, due to the cost of pointer construction
	else if (M == 0.0) {
		warning("Sum of masses is zero - {"+green+"m1"+res+"}: "+scientific(m1)+" {"+green+"m2"+res+"}. Returning (0,0,0).", __LINE__, __FILE__);
		return Pos();
	}
	else {
		error("Sum of masses is negative - {"+green+"m1"+res+"}: "+scientific(m1)+" {"+green+"m2"+res+"}. Returning (0,0,0).", __LINE__, __FILE__);
		return Pos();
	}
}
double CBody::distance(CBody* target) {
	Pos t = target -> pos;
	double dist = sqrt(pow(t.x - pos.x,2) + pow(t.y - pos.y, 2) + pow(t.z - pos.z, 2));
	printrln(in("CBody", "distance")+"Distance from "+pos.info(2)+" to "+t.info(2)+" is ", scientific(dist, 3)+res, 4);
	return dist;
}
double CBody::distance(CBody target) const {
	Pos t = target.pos;
	double dist = sqrt(pow(t.x - pos.x,2) + pow(t.y - pos.y, 2) + pow(t.z - pos.z, 2));
	printrln(in("CBody", "distance")+"Distance from "+pos.info(2)+" to "+t.info(2)+" is ", scientific(dist, 3)+res, 4);
	return dist;	
}
double CBody::distance(Pos* t) {
	double dist = sqrt(pow(t -> x - pos.x,2) + pow(t -> y - pos.y, 2) + pow(t -> z - pos.z, 2));
	printrln(in("CBody", "distance")+"Distance from "+pos.info()+" to "+t -> info()+" is ", scientific(dist)+res, 4);
	return dist;
}
double CBody::distance(Pos t) const {
	return sqrt(pow(t.x - pos.x, 2) + pow(t.y - pos.y, 2) + pow(t.z - pos.z, 2));
}

CBody::BulirschStoer::BulirschStoer(CBody* body) {
	this->body = body;
}

void CBody::init() {
	print("Initializing new "+cyan+bright+"CBody"+res+"...");

	parent = NULL;
	name = "";
	type = bodyType::def;
	BS = new BulirschStoer(this);
	
	radius = 0;
	x = 0;
	y = 0;
	z = 0;
	xv = 0;
	yv = 0;
	zv = 0;
	mass = 0;
	speed = 0;

	net = Force(0,0,0);
	pos = Pos(0,0,0);
	h = 1;
	fix = 0;

	ncalcs = 0;
	totSteps = 0;
	
	print(green+"done\n"+res);
}

std::string CBody::info() {
	std::string s = "";
	s.append("Body "+name+"\n");
	s.append("\t Position "+pos.info()+"\n");
	s.append("\t Radius   "+scientific(radius, 5)+"\n");
	s.append("\t Mass     "+scientific(mass, 5)+"\n");
	return s;
}

std::string CBody::writeFormat(format f) {
	std::string formatted = "";
	switch(f) {
		case text:
			formatted.append("Body - "+Name()+"\n");															// Exclude
			formatted.append("   Position "+scientific(x,5)+" "+scientific(y,5)+" "+scientific(z,5)+"\n"); 		// Exclude
			formatted.append("   Radius   "+scientific(radius)+"\n");											// Exclude
			formatted.append("   Mass     "+scientific(mass)+"\n");												// Exclude
			formatted.append("   Velocity "+scientific(xv,5)+" "+scientific(yv,5)+" "+scientific(zv,5)+"\n\n");	// Exclude
			break;
	}
	return formatted;
}

bool CBody::operator != (CBody r) const {
	if (r == *this) {
		return false;
	}
	else {
		return true;
	}
}
bool CBody::operator == (CBody r) const {
	if (r.pos == pos && r.Mass() == mass) {
		return true;
	}
	else {
		return false;
	}
}
