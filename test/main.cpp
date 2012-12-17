/*
#include <websocketpp/websocketpp.hpp> // this is a convenience header that includes only the basics
#include <sstream>
using websocketpp::server;

struct echo_server_handler : public server::handler {
	std::map<connection_ptr,std::string> m_connections;
	void on_open(connection_ptr con) {
	    std::cout << "client " << con << " joined the lobby." << std::endl;
	    m_connections.insert(std::pair<connection_ptr,std::string>(con,get_con_id(con)));
	}

	void on_message(connection_ptr connection,message_ptr msg) {
		std::vector<std::string> tab;
		std::string s = msg->get_payload();
		s = s.substr(1,s.length()-1);
		split(s,':',tab);
		//std::cout << "message from client " << connection << ": " << atoi(tab[1].c_str()) << std::endl;
		int nbIt = atoi(tab[1].c_str());
		long final =0;
		for(int i = 0 ; i < nbIt; i++)
			for(int z = 0; z < nbIt; z++)
				final+= (i-nbIt)*z;
		//usleep(100);

		std::string number;
		std::stringstream strstream;
		strstream << final;
		strstream >> number;

		std::string mess = "{nb : \""+number+"\"}";
		connection->send(mess,msg->get_opcode());



	}

	std::string get_con_id(connection_ptr con) {
	    std::stringstream endpoint;
	    //endpoint << con->get_endpoint();
	    endpoint << con;
	    return endpoint.str();
	}

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	    std::stringstream ss(s);
	    std::string item;
	    while(std::getline(ss, item, delim)) {
	        elems.push_back(item);
	    }
	    return elems;
	}

	std::vector<std::string> split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    return split(s, delim, elems);
	}
};

int main() {
	// create a shared pointer to an echo_server_handler
	server::handler_ptr echo_handler(new echo_server_handler());
	// create a server endpoint and initialize with the new handler
	server echo_endpoint(echo_handler);
	// instruct the endpoint to listen for connections on port 9002
	// this line will block until something within the endpoint event loop asks it to stop.
	echo_endpoint.listen(9002);
	return 0;
}
*/
