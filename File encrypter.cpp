#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>

using namespace std;

//This function will encrypt a single file
void encrypt_file(string filePath, string password) {
	//Convert password to the sum of the decimal value of each letter
	int sum = 0;
	for (char c : password) {
		sum += static_cast<int>(c);
	}

	//tempFile is a temporary file which will save encrypted data of the file
	fstream file, tempFile;
	string tempFilePath = "temp.txt";

	//Open file to read / temp file to write
	file.open(filePath, ios::in | ios::binary);
	tempFile.open(tempFilePath, ios::out | ios::binary);

	//Read byte by byte through file
	char byte;
	while (file.get(byte)) {
		//Encrypt this byteby adding 1 to it
		byte += sum;
		//Save this byte to the temp file
		tempFile.put(byte);
	}

	//Close file
	file.close();
	tempFile.close();

	//Open file to write / temp file to read
	file.open(filePath, ios::out | ios::binary);
	tempFile.open(tempFilePath, ios::in | ios::binary);

	//Read byte by byte through temp file 
	char byte2;
	while (tempFile.get(byte2)) {
		//Save this byte into file
		file.put(byte2);
	}

	//Close files
	file.close();
	tempFile.close();

	//Delete temp file
	remove(tempFilePath.c_str());
}

//This function will dencrypt a single file
void decrypt_file(string filePath, string password) {
	//Convert password to the sum of the decimal value of each letter
	int sum = 0;
	for (char c : password) {
		sum += static_cast<int>(c);
	}

	//tempFile is a temporary file which will save dencrypted data of the file
	fstream file, tempFile;
	string tempFilePath = "temp.txt";

	//Open file to read / temp file to write
	file.open(filePath, ios::in | ios::binary);
	tempFile.open(tempFilePath, ios::out | ios::binary);

	//Read byte by byte through file
	char byte;
	while (file.get(byte)) {
		//Dencrypt this byteby adding 1 to it
		byte -= sum;
		//Save this byte to the temp file
		tempFile.put(byte);
	}

	//Close file
	file.close();
	tempFile.close();

	//Open file to write / temp file to read
	file.open(filePath, ios::out | ios::binary);
	tempFile.open(tempFilePath, ios::in | ios::binary);

	//Read byte by byte through temp file 
	char byte2;
	while (tempFile.get(byte2)) {
		//Save this byte into file
		file.put(byte2);
	}

	//Close files
	file.close();
	tempFile.close();

	//Delete temp file
	remove(tempFilePath.c_str());
}

int main() {
	int choice;
	string password;
	string filepath;
	bool go_again = true;
	char restart;

	while (go_again == true) {
		system("cls");
		cout << "1) Encrypt\n2) Decrypt" << endl;
		cout << "\nChoise: ";
		if (cin >> choice && choice == 1) {
			cout << "Enter a password for encryption: ";
			cin >> password;
			system("cls");
			cout << "Enter the path of the file you want to encrypt:";
			cin >> filepath;
			cout << "Encrypting..." << endl;
			encrypt_file(filepath, password);
			system("cls");
			cout << "Done!" << endl;
		}
		else if (choice == 2) {
			cout << "Enter a password for decryption: ";
			cin >> password;
			system("cls");
			cout << "Enter the path of the file you want to decrypt:";
			cin >> filepath;
			cout << "Decrypting..." << endl;
			decrypt_file(filepath, password);
			system("cls");
			cout << "Done!" << endl;
		}

		go_again = false;
		cout << "Go again? y/n: ";
		if (cin >> restart && restart == 'y')
			go_again = true;
		else if (restart == false)
			go_again = false;
	}

	return 0;
}