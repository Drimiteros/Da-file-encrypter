#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>

using namespace std;

int generate_key(string password) {
	//Convert password to the sum of the decimal value of each letter
	int sum = 0;
	for (char c : password) {
		sum += static_cast<int>(c);
	}
	return sum;
}

void encrypt_decrypt(vector<unsigned char>& buffer, fstream& file, fstream& tempFile, int choice, int key) {
	//Process file in chunks
	while (file.read(reinterpret_cast<char*>(buffer.data()), buffer.size()) || file.gcount() > 0) {
		size_t bytesRead = file.gcount();
		for (size_t i = 0; i < bytesRead; i++) {
			if (choice == 1)
				buffer[i] = buffer[i] + key;
			if (choice == 2)
				buffer[i] = buffer[i] - key;
		}
		tempFile.write(reinterpret_cast<char*>(buffer.data()), bytesRead);
	}
}

//This function will encrypt / decrypt a single file
void open_file(string filePath, string password, int choice) {
	//Get the password's sum of the decimal value of each letter to create the key and use it to encrypt / decrypt the byte
	int key = generate_key(password);

	//Open file to read / temp file to write
	//tempFile is a temporary file which will save encrypted data of the file
	fstream file, tempFile;
	string tempFilePath = "temp.txt";
	file.open(filePath, ios::in | ios::binary);
	tempFile.open(tempFilePath, ios::out | ios::binary);

	//Encrypt / Decrypt bytes by chunks
	const size_t buffer_size = 65536; //64 KB chunks
	vector<unsigned char> buffer(buffer_size);
	encrypt_decrypt(buffer, file, tempFile, choice, key);

	//Close files
	file.close();
	tempFile.close();

	//Open file to write / temp file to read
	file.open(filePath, ios::out | ios::binary);
	tempFile.open(tempFilePath, ios::in | ios::binary);

	while (tempFile.read(reinterpret_cast<char*>(buffer.data()), buffer.size()) || tempFile.gcount() > 0)
		file.write(reinterpret_cast<char*>(buffer.data()), tempFile.gcount());

	//Close files
	file.close();
	tempFile.close();
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
		cout << "\nChoice: ";
		if (cin >> choice && choice == 1) {
			cout << "Enter a password for encryption: ";
			cin >> password;
			system("cls");
			cout << "Enter the path of the file you want to encrypt:";
			cin >> filepath;
			cout << "Encrypting..." << endl;
		}
		else if (choice == 2) {
			cout << "Enter a password for decryption: ";
			cin >> password;
			system("cls");
			cout << "Enter the path of the file you want to decrypt:";
			cin >> filepath;
			cout << "Decrypting..." << endl;
		}

		open_file(filepath, password, choice);
		system("cls");
		cout << "Done!" << endl;

		go_again = false;
		cout << "Go again? y/n: ";
		if (cin >> restart && restart == 'y')
			go_again = true;
		else if (restart == false)
			go_again = false;
	}

	return 0;
}