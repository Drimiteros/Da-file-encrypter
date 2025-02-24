#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

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
void open_file(string filePath, string password, int choice, int chunk, chrono::duration<double>& sec) {
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
	if (fileSize >= pow(1024, 3))
		cout << "File size: " << fixed << setprecision(2) << fileSize / pow(1024, 3) << " GB" << endl;
	else if (fileSize >= pow(1024, 2))
		cout << "File size: " << fixed << setprecision(2) << fileSize / pow(1024, 2) << " MB" << endl;
	else if (fileSize >= 1024)
		cout << "File size: " << fixed << setprecision(2) << fileSize / 1024 << " KB" << endl;
	else
		cout << "File size: " << fixed << setprecision(2) << fileSize << " B" << endl;

	//Encrypt / Decrypt bytes by chunks
	size_t buffer_size;
	if (chunk == 0)
		buffer_size = 65536; //64 KB chunks
	else
		buffer_size = chunk;
	vector<unsigned char> buffer(buffer_size);
	//Get the time to process
	auto start = chrono::steady_clock::now();
	encrypt_decrypt(fileSize, buffer, file, tempFile, choice, key);
	auto end = chrono::steady_clock::now();
	sec = end - start;

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

void open_directory(string filePath, string password, int choice, int chunk) {
	for (const auto& entry : filesystem::directory_iterator(filePath)) {
		int key = generate_key(password);

		try {
			if (entry.is_regular_file()) {  
				//cout << entry.path() << endl;
				cout << entry.path().filename() << endl;
			}
			else if (entry.is_directory()) {  
				for (const auto& entry2 : filesystem::recursive_directory_iterator(entry.path(), filesystem::directory_options::skip_permission_denied)) {
					try {
						if (entry2.is_regular_file()) {
							//cout << entry.path() << endl;
							cout << entry2.path().filename() << endl;
						}
					}
					catch (const exception& e) {
						wcout << "Error: " << e.what() << endl;
						continue;
					}
				}
			}
		}
		catch (const exception& e) {
			wcout << "Error: " << e.what() << endl;
		}
	}
}

int main() {
	int choice;
	int chunk;
	string password;
	string filepath;
	bool go_again = true;
	bool process_directory = false;
	char restart;
	chrono::duration<double> sec;

	while (go_again == true) {
		system("cls");
		cout << "1) Encrypt\n2) Decrypt" << endl;
		cout << "\nChoice: ";
		cin >> choice;
		system("cls");
		cout << "Enter the chunk size in Bytes or type \"0\" to use the default 64 KB size\n(performs calculations per 64 KB of data in each iteration.\n"
			"Bigger chunks help speed up calculations for large files)\n\nExamples:\n1 KB = 1024 B\n120 KB = 122880 B\n500 MB = 524288000 B\n1 GB = 1073741824 B\n\nChoice: ";
		cin >> chunk;
		system("cls");
		if (choice == 1) {
			cout << "Enter a password for encryption: ";
			cin >> password;
			system("cls");
			cout << "0) Encrypt single file\n1) Encrypt entire directory\n";
			cout << "\nChoice: ";
			cin >> process_directory;
			system("cls");
			cout << "Enter the path:";
			cin >> filepath;
			cout << "Encrypting..." << endl;
		}
		else if (choice == 2) {
			cout << "Enter a password for decryption: ";
			cin >> password;
			system("cls");
			cout << "0) Decrypt single file\n1) Decrypt entyre directory\n";
			cout << "\nChoice: ";
			cin >> process_directory;
			system("cls");
			cout << "Enter the path:";
			cin >> filepath;
			cout << "Decrypting..." << endl;
		}

		if (process_directory == false)
			open_file(filepath, password, choice, chunk, sec);
		if (process_directory == true)
			open_directory(filepath, password, choice, chunk);
		system("cls");
		cout << "Time to execute: " << sec << endl;
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