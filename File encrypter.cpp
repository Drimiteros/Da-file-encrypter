#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

int generate_key(string password) {
	//Convert password to the sum of the decimal value of each letter
	int sum = 0;
	for (char c : password) {
		sum += static_cast<int>(c);
	}
	return sum;
}

//This function will encrypt / decrypt a single file
void encrypt_decrypt(uintmax_t fileSize, vector<unsigned char>& buffer, fstream& file, fstream& tempFile, int choice, int key) {
	int totalProcessed = 0;
	
	//Process file in chunks
	while (file.read(reinterpret_cast<char*>(buffer.data()), buffer.size()) || file.gcount() > 0) {
		size_t bytesRead = file.gcount();
		for (size_t i = 0; i < bytesRead; i++) {
			//Encrypt
			if (choice == 1)
				buffer[i] = buffer[i] + key;
			//Decrypt
			if (choice == 2)
				buffer[i] = buffer[i] - key;
		}

		// Calculate and display progress
		totalProcessed += bytesRead;
		double progress = (static_cast<double>(totalProcessed) / fileSize) * 100;
		cout << "\rCompleted: " << fixed << setprecision(2) << progress << "%   " << flush; // Overwrites the same line

		tempFile.write(reinterpret_cast<char*>(buffer.data()), bytesRead);
	}
}

//This function locates the current file
void open_file(string filePath, string password, int choice, int chunk) {
	//Get the password's sum of the decimal value of each letter to create the key and use it to encrypt / decrypt the byte
	int key = generate_key(password);

	//Open file to read / temp file to write
	//tempFile is a temporary file which will save encrypted data of the file
	fstream file, tempFile;
	string tempFilePath = "temp.txt";
	file.open(filePath, ios::in | ios::binary);
	tempFile.open(tempFilePath, ios::out | ios::binary);

	//Get the file size
	system("cls");
	uintmax_t fileSize = filesystem::file_size(filePath);
	if (fileSize < pow(1024, 3))
		cout << "File size: " << setprecision(2) << fileSize / pow(1024, 2) << " MB" << endl;
	else
		cout << "File size: " << setprecision(2) << fileSize / pow(1024, 3) << " GB" << endl;

	//Encrypt / Decrypt bytes by chunks
	size_t buffer_size;
	if (chunk == 0)
		buffer_size = 65536; //64 KB chunks
	else
		buffer_size = chunk;
	vector<unsigned char> buffer(buffer_size);
	encrypt_decrypt(fileSize, buffer, file, tempFile, choice, key);

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

void encrypt_directory() {

}

int main() {
	int choice;
	int chunk;
	string password;
	string filepath;
	bool go_again = true;
	char restart;

	while (go_again == true) {
		system("cls");
		cout << "1) Encrypt\n2) Decrypt" << endl;
		cout << "\nChoice: ";
		cin >> choice;
		system("cls");
		cout << "Enter the chunk size in Bytes or type \"0\" to use the default 64 KB size\n(performs calculations per 64 KB of data in each iteration.\nBigger chunks help speed up calculations for large files): ";
		cin >> chunk;
		system("cls");
		if (choice == 1) {
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

		open_file(filepath, password, choice, chunk);
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