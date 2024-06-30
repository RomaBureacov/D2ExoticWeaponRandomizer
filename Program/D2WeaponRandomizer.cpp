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

#include<chrono>
#include<thread>

using namespace std;

struct weapon {
	string name,
		element,
		ammo,
		slot,
		type;
};

// quick regex search
bool search(const vector<string> &params, string attribute) {
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

vector<string> getDesired(const vector<string> options, string type) {
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

bool containing(const vector<string> &vect, const string& key) {
	for (string hole : vect) {
		if (key == hole) return true;
	}
	return false;
}

int main() {
	// create the master list
	ifstream stream("WeaponsList.csv");
	
	if (!stream) {
		cout << "Failed to find WeaponsList.csv, Exiting Program..." << endl;
		system("Pause");
		return -1;
	}
	
	vector<weapon> master = buildMaster(stream);
	
	string input;
	do {
		// ask the user for input	
		vector<string> elements = { "Kinetic", "Stasis", "Strand" }; // assume initially that the user only wants kinetic-slot weapons
		vector<string> slots = { "Kinetic", "Energy", "Power" };
		vector<string> ammo = { "Primary", "Special", "Power" };
		vector<string> primaries = { "Hand Cannon", "Sidearm", "Bow", "Scout Rifle", "Pulse Rifle", "Auto Rifle", "Submachine Gun" };
		vector<string> specials = { "Sniper Rifle", "Breech Grenade launcher", "Shotgun", "Trace Rifle", "Fusion Rifle", "Glaive",
									"Sidearm", "Hand Cannon", "Linear Fusion Rifle", "Sword"}; // exceptions
		vector<string> heavies = { "Machine Gun", "Rocket Launcher", "Sword", "Grenade Launcher", "Linear Fusion Rifle",
									"Fusion Rifle", "Sniper Rifle", "Shotgun", "Bow", "Glaive", "Trace Rifle"}; // exceptions

		// get the desired slots for the weapons
		vector<string> desired_slots = getDesired(slots, "slots");

		// get the desired elements for the weapons
		if (containing(desired_slots, "Energy") || containing(desired_slots, "Power")) { // include other elements should energy and/or Power slots are desired
			elements.push_back("Solar");
			elements.push_back("Void");
			elements.push_back("Arc");
		}
		if (!containing(desired_slots, "Kinetic")) { // if kinetic-slot is excluded in any way
			elements.erase(elements.begin() + 0); // remove kinetic option
		}
		vector<string> desired_elements = getDesired(elements, "elements");

		// get the desired ammos for the weapons
		vector<string> desired_ammos;

		if (!containing(desired_slots, "Kinetic") && !containing(desired_slots, "Energy")) { // exclude primary and special ammo should their slots not be desired
			ammo.erase(ammo.begin() + 0);
			ammo.erase(ammo.begin() + 0);
			desired_ammos = ammo;
		} else  {
			if (!containing(desired_slots, "Power"))
				ammo.erase(ammo.begin() + 2); // exclude power ammo should the power slot not be desired

			desired_ammos = getDesired(ammo, "ammos");
		}

		// get the desired types for the weapons
		vector<string> types;
		if (containing(desired_ammos, "Primary")) types.insert(types.begin(), primaries.begin(), primaries.end());
		if (containing(desired_ammos, "Special")) types.insert(types.begin(), specials.begin(), specials.end());
		if (containing(desired_ammos, "Power")) types.insert(types.begin(), heavies.begin(), heavies.end());
		
		vector<string> desired_types = getDesired(types, "types");


		// build the list of choices
		vector<string> list = buildList(master, desired_elements, desired_slots, desired_ammos, desired_types);

		if (list.size() == 0) {
			cout << "There are 0 possible rolls, please try a different set of parameters." << endl;
		} else {
			srand(time(0));

			do {
				cout << list.size() << " possible roll" << (list.size() > 1 ? "s" : "") << "; You rolled: ";
				for (int i = 0; i < 3; i++) {
					this_thread::sleep_for(chrono::milliseconds(1000));
					cout << ". ";
				}
				this_thread::sleep_for(chrono::milliseconds(1000));
				cout << list[rand() % list.size()] << "!" << endl;
				cout << "Roll again? (Y/N): "; cin >> input;
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