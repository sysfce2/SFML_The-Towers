#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include "Structs.hpp"

// Forward Declarations
uint8_t createUniqueID();
bool doesIDExist(uint8_t id);
void addClient(sf::TcpListener& listener, sf::SocketSelector& selector);
void udpThread();
void compressAndSendWorld();
void freeWorldData();
uint8_t getReceivedPacket(sf::SocketSelector& selector, sf::Packet& packet, unsigned int& _senderIndex);
void sendPacketToOtherClients(sf::Packet& packet, uint8_t senderID);
void sendPacketToAllClients(sf::Packet& packet);
void disconnectPlayer(sf::SocketSelector& _selector, unsigned int _playerID);
void generateWorld();
void setBlock(int _x, int _y, int _z);
void updateWorldWithBlockUpdatePacket(sf::Packet& _packet);
void addCodeToBlockUpdatePacket(sf::Packet& _packet);


// Global Variables
uint8_t* worldData = nullptr;
std::vector<Client> clients;
bool isDone = false;

const unsigned int WORLD_LENGTH = 4;
const unsigned int WORLD_WIDTH = 8;
const unsigned int WORLD_HEIGHT = 2;
const unsigned int CHUNK_WIDTH = 32;
const unsigned int CHUNK_SIZE = CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH;
const unsigned int CLIENT_PORT = 7459;
const unsigned int SERVER_PORT = 7456;

int main(){
	generateWorld();

	// Server variables
	sf::TcpListener listener;
	sf::SocketSelector selector;

	// Starting server
	std::cout << "Listening for connection..." << std::endl;
	listener.listen(SERVER_PORT);
	selector.add(listener);

	// Starting threads
	std::thread positionUpdater(udpThread); // Starting packet position thread

	while(!isDone){
		if(selector.wait()){ // Wait for event to happen
			if(selector.isReady(listener)){ // Got new connection, so we are going to handle that by creating a new client
				addClient(listener, selector);
				compressAndSendWorld();
			} else { // Got data from a connected client so we are going to send it to all other clients
				sf::Packet receivedPacket;
				unsigned int senderIndex;
				uint8_t code = getReceivedPacket(selector, receivedPacket, senderIndex);

				switch(code){
					case 1: // Disconnect
						disconnectPlayer(selector, senderIndex);
					break;
					case 2: // Block Update
						updateWorldWithBlockUpdatePacket(receivedPacket);
						addCodeToBlockUpdatePacket(receivedPacket);
						sendPacketToOtherClients(receivedPacket, senderIndex);
					break;
				}
			}
		}
	}

	positionUpdater.join();
	freeWorldData();

	return 0;
}

void udpThread(){
	//Variables for algorithm
	sf::UdpSocket socket;
	sf::Packet receivedPacket;
	sf::IpAddress remoteIp;
	unsigned short remotePort;

	//Initializing variables
	socket.bind(SERVER_PORT);
	socket.setBlocking(false);

	while(!isDone){
		receivedPacket.clear();

		while(socket.receive(receivedPacket, remoteIp, remotePort) == sf::Socket::Done){
			uint8_t id;
			receivedPacket >> id;
			receivedPacket << id;

			for(auto& i : clients){
				if(i.id != id) socket.send(receivedPacket, i.socket->getRemoteAddress(), CLIENT_PORT);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void generateWorld(){
	// Allocate memory for the world
	worldData = static_cast<uint8_t*>(malloc(WORLD_WIDTH * WORLD_LENGTH * WORLD_HEIGHT * CHUNK_SIZE));

	unsigned int maxW = WORLD_WIDTH * CHUNK_WIDTH;
	unsigned int maxL = WORLD_LENGTH * CHUNK_WIDTH;

	// Fill in the memory
	for(unsigned int y = 0; y < WORLD_HEIGHT * CHUNK_WIDTH; y++){
		for(unsigned int z = 0; z < WORLD_LENGTH * CHUNK_WIDTH; z++){
			for(unsigned int x = 0; x < WORLD_WIDTH * CHUNK_WIDTH; x++){
				if(y < 20){
					worldData[(y * maxW * maxL) + (z * maxW) + x] = 5;
				}else{
					worldData[(y * maxW * maxL) + (z * maxW) + x] = 0;
				}
				if(x == z){
					worldData[(y * maxW * maxL) + (z * maxW) + x] = 2;
				}
			}
		}
	}
}

uint8_t createUniqueID(){
	uint8_t id = rand()%255 + 1;
	if(!doesIDExist(id)){
		return id;
	}
	return createUniqueID();
}

bool doesIDExist(uint8_t id){
	for(auto& i : clients){
		if(i.id == id) return true;
	}
	return false;
}

void addClient(sf::TcpListener& listener, sf::SocketSelector& selector) {
	// Adding new client to list of clients
	Client client;
	listener.accept(*client.socket);
	client.id = createUniqueID();
	clients.push_back(client);
	selector.add(*client.socket);

	// Sending chosen ID to client
	sf::Packet packet;
	packet << client.id;
	client.socket->send(packet);
	packet.clear();
	std::cout << "New client connected with ID: " << (unsigned int)client.id << std::endl;
}

void freeWorldData(){
	free(worldData);
}

void compressAndSendWorld(){
	sf::Packet packet;
	packet.clear();

	// Compressing the world into a packet
	uint32_t numBlocks = 1;
	for(uint32_t i = 1; i < (WORLD_WIDTH * WORLD_LENGTH * WORLD_HEIGHT * CHUNK_SIZE); i++){
		if(worldData[i - 1] != worldData[i]){
			packet << (uint8_t)worldData[i - 1] << numBlocks;
			numBlocks = 1;
		}else{
			numBlocks++;
		}
	}
	packet << (uint8_t)worldData[WORLD_WIDTH * WORLD_LENGTH * WORLD_HEIGHT * CHUNK_SIZE - 1] << numBlocks;

	//Sending the packet containing the compressed world to the newly connected client
	clients.back().socket->send(packet);
}

uint8_t getReceivedPacket(sf::SocketSelector& selector, sf::Packet& packet, unsigned int& _senderIndex) {
	for(unsigned int i = 0; i < clients.size(); i++){
		if(selector.isReady(*clients[i].socket)){
			sf::Socket::Status status = clients[i].socket->receive(packet);
			if(status == sf::Socket::Done){ // Got a valid packet
				_senderIndex = i;
				uint8_t code;
				packet >> code;
				return code;
			} else if (status == sf::Socket::Disconnected) { // The client has disconnected
				// We remove the client and send a packet to other clients that a player has disconnected
				_senderIndex = i;
				return 1; // We return 1 because 1 is the code for a disconnect request
			}
		}
	}
	return 0;
}

void disconnectPlayer(sf::SocketSelector& _selector, unsigned int _playerID){
	sf::Packet packet;
	packet.clear();
	packet << (uint8_t)1 << clients[_playerID].id; // We send the keycode 1 because that is the code for a disconnection
	sendPacketToAllClients(packet);
	std::cout << "Client Disconnected with ID: " << (unsigned int)clients[_playerID].id << std::endl;
	_selector.remove(*clients[_playerID].socket);
	delete clients[_playerID].socket;
	clients[_playerID] = clients.back();
	clients.pop_back();
}

void sendPacketToAllClients(sf::Packet& packet){
	for(auto& i : clients){
		i.socket->send(packet);
	}
}

void sendPacketToOtherClients(sf::Packet& packet, uint8_t senderID){
	for(auto& i : clients){
		if(i.id != senderID) i.socket->send(packet);
	}
}

void setBlock(int _x, int _y, int _z, uint8_t _block){
	unsigned int maxW = WORLD_WIDTH * CHUNK_WIDTH;
	unsigned int maxL = WORLD_LENGTH * CHUNK_WIDTH;
	worldData[(_y * maxW * maxL) + (_z * maxW) + _x] = _block;
}

void updateWorldWithBlockUpdatePacket(sf::Packet& _packet){
	int x = 0;
	int y = 0;
	int z = 0;
	uint8_t block = 0;

	_packet >> x >> y >> z >> block;

	setBlock(x, y, z, block);
}

void addCodeToBlockUpdatePacket(sf::Packet& _packet){
	int x, y, z;
	uint8_t block;

	_packet >> x >> y >> z >> block;
	_packet.clear();
	_packet << (uint8_t)2 << x << y << z << block;
}
