/**
* take some user input specifying the parameters
* 
* build a list of weapons matching the parameters
* 
* give a random number of weapons based on the list
* 
* repeat process if necessary
* 
* APPEND NEW WEAPONS IF THEY DO NOT EXIST IN THE CSV
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<regex>
#include<cstdlib>
#include<ctime>
#include<sstream>

#include<random>

#include<chrono>
#include<thread>

using namespace std;

struct weapon {
	string 
		name,
		element,
		ammo,
		slot,
		type;
};

struct params {
	vector<string> 
		elements,
		slots,
		ammo,
		primaries,
		specials,
		heavies;
};

// prototype functions
bool search(const vector<string>&, const string&);
vector<string> buildList(const vector<weapon>&, const vector<string>&, const vector<string>&, const vector<string>&, const vector<string>&);
vector<weapon> buildMaster(ifstream&);
params buildParams(const vector<weapon>&);
vector<string> getDesired(const vector<string>&, const string&);
bool containing(const vector<string>&, const string&);
int rInt(const int &, const int &);

// quick regex search
bool search(const vector<string> &params, const string &attribute) {
	for (const string& pa : params) { // for each element specified
		regex rx(pa, regex_constants::icase);
		if (regex_search(attribute, rx)) return true;
	}
	return false; // exhausted possible items
}

// build the appropriate list using the parameters
vector<string> buildList(const vector<weapon>& master, const vector<string>& element, 
						 const vector<string>& slot, const vector<string>& ammo, const vector<string>& type) {
	vector<string> product;
	
	// build the list using the restriction specified
	for (const weapon& w : master) { // for each weapon
		if (w.element.find("/") != string::npos) { // if weapon is multi-elemental

			if (search(slot, w.slot) // first check if the weapon matches other criteria
				&& search(ammo, w.ammo)
				&& search(type, w.type)) {
				// check each element if it's in the desired elements
				vector<string> elements;
				string token;
				istringstream tokenStream(w.element);
				while (getline(tokenStream, token, '/' ))
					elements.push_back(token);
				
				for (const string &weapon_subElement : elements) { // search for each element in desired list
					
					if (search(element, weapon_subElement)) {
						product.push_back(w.name);
						break;
					}
				}

			}
		} else if (search(element, w.element)
			&& search(slot, w.slot)
			&& search(ammo, w.ammo)
			&& search(type, w.type))
			product.push_back(w.name);
	}

	return product;
}

// build the master list
vector<weapon> buildMaster(ifstream& stream) {
	vector<weapon> master;
	string line;

	getline(stream, line); // skip first line to pass the Byte Order Mark (BOM)
	while (getline(stream, line)) {
		if (line.empty() || line[0] == '#') continue; // skip if line is empty or comment

		vector<string> tokens;
		string token;
		istringstream tokenStream(line);
		while (getline(tokenStream, token, ',' ))
			tokens.push_back(token);

		weapon item;
		item.name = tokens[0];
		item.element = tokens[1];
		item.slot = tokens[2];
		item.ammo = tokens[3];
		item.type = tokens[4];

		master.push_back(item);
	}

	return master;
}

// build params primary, special, and power weapon types lists based off of the master list
params buildParams(const vector<weapon> &master) {
	params parameters;
	parameters.elements = { "Kinetic", "Stasis", "Strand" }; // assume initially that the user only wants kinetic- or power-slot weapons
	parameters.slots = { "Kinetic", "Energy", "Power" };
	parameters.ammo = { "Primary", "Special", "Power" };

	// build weapon types
	for (weapon w : master) {
		if (w.ammo == "Primary" && !containing(parameters.primaries, w.type)) parameters.primaries.push_back(w.type);
		else if (w.ammo == "Special" && !containing(parameters.specials, w.type)) parameters.specials.push_back(w.type);
		else if (w.ammo == "Power" && !containing(parameters.heavies, w.type)) parameters.heavies.push_back(w.type);	
	}

	return parameters;
}

vector<string> getDesired(const vector<string> &options, const string &type) {
	string input;
	vector<string> desiredOptions;

	bool chooseAll = false;
	bool nullAll = false;

	cout << "Enter desired " << type << " (Y/N/Any/Null): " << endl;
	do {
		for (string op : options) {
			if (chooseAll) desiredOptions.push_back(op);
			else if (nullAll); // do nothing
			else {
				do {
					cout << "|" << op << ": "; cin >> input;
					if (input == "y" || input == "Y") desiredOptions.push_back(op);
					else if (input == "any" || input == "Any") {
						chooseAll = true;
						desiredOptions.push_back(op);
					} else if (input == "null" || input == "Null") {
						if (desiredOptions.size() == 0) {
							cout << "|ERROR: You must choose at least 1 option before nullifying the rest" << endl;
						} else 
							nullAll = true;
					}
				} while (input != "y" && input != "Y"
					   && input != "n" && input != "N"
					   && input != "any" && input != "Any"
					   && input != "null" && input != "Null" // repeat while input invalid
					   || desiredOptions.size() == 0 && (input == "null" || input == "Null")); // and while the options list is empty and null is decided
			}
		}
	} while (desiredOptions.size() <= 0);
	cout << endl;

	return desiredOptions;
}

// find string key in vector vect
bool containing(const vector<string> &vect, const string &key) {
	if (vect.size() == 0) return false;

	for (string hole : vect) {
		if (key == hole) return true;
	}
	return false;
}

// create a random integer
int rInt(const int &lowerBound, const int &upperBound) {
	random_device device;
	mt19937 engine(device());
	uniform_int_distribution<> distribution(lowerBound, upperBound);
	return distribution(engine);
}

int main() {
	// create the master list
	ifstream stream("WeaponsList.csv");

	if (!stream) {
		cout << "Failed to find WeaponsList.csv, Exiting Program..." << endl;
		system("Pause");
		return -1;
	}

	// build master list
	vector<weapon> master = buildMaster(stream);

	// build parameters list
	params parameters = buildParams(master);


	string input;
	do {
		// ask the user for input	
		// create copy of parameters
		params parameters_copy = parameters;

		// get the desired slots for the weapons
		vector<string> desired_slots = getDesired(parameters_copy.slots, "slots");

		// get the desired elements for the weapons
		if (containing(desired_slots, "Energy") || containing(desired_slots, "Power")) { // include other elements should energy and/or Power slots are desired
			if (desired_slots.size() == 1 && containing(desired_slots, "Energy")) { // if only energy weapons are desired, exlude kinetic, stasis, strand
				parameters_copy.elements.clear();
			}
			
			parameters_copy.elements.push_back("Solar");
			parameters_copy.elements.push_back("Void");
			parameters_copy.elements.push_back("Arc");
		}
		vector<string> desired_elements = getDesired(parameters_copy.elements, "elements");

		// get the desired ammos for the weapons
		vector<string> desired_ammos;

		if (!containing(desired_slots, "Kinetic") && !containing(desired_slots, "Energy")) { // exclude primary and special ammo should their slots not be desired
			parameters_copy.ammo.erase(parameters_copy.ammo.begin() + 0);
			parameters_copy.ammo.erase(parameters_copy.ammo.begin() + 0);
			desired_ammos = parameters_copy.ammo;
		} else {
			if (!containing(desired_slots, "Power"))
				parameters_copy.ammo.erase(parameters_copy.ammo.begin() + 2); // exclude power ammo should the power slot not be desired

			desired_ammos = getDesired(parameters_copy.ammo, "ammos");
		}

		// get the desired types for the weapons
		vector<string> types;
		if (containing(desired_ammos, "Primary")) types.insert(types.begin(), parameters_copy.primaries.begin(), parameters_copy.primaries.end());
		if (containing(desired_ammos, "Special")) types.insert(types.begin(), parameters_copy.specials.begin(), parameters_copy.specials.end());
		if (containing(desired_ammos, "Power")) types.insert(types.begin(), parameters_copy.heavies.begin(), parameters_copy.heavies.end());

		vector<string> desired_types = getDesired(types, "types");


		// build the list of choices
		vector<string> list = buildList(master, desired_elements, desired_slots, desired_ammos, desired_types);

		if (list.size() == 0) {
			cout << "There are 0 possible rolls, please try a different set of parameters." << endl;
		} else {
			do {
				cout << list.size() << " possible roll" << (list.size() > 1 ? "s" : "") << "; You rolled: ";
				for (int i = 0; i < 3; i++) {
					this_thread::sleep_for(chrono::milliseconds(1000));
					cout << ". ";
				}
				this_thread::sleep_for(chrono::milliseconds(1000));
				cout << list[rInt(0, list.size() - 1)] << "!" << endl;
				do {
					cout << "|Roll again? (Y/N): "; cin >> input;
				} while ((input != "Y" && input != "y") && (input != "N" && input != "n"));
			} while (input != "N" && input != "n");
		}

		// prompt to restart
		do {
			cout << endl << endl << "Would you like to change parameters? (y/quit)" << endl;
			cin >> input; 
			cout << endl;
		} while (input != "y" && input != "quit");
		

	} while (input != "quit");

	return 0;
}