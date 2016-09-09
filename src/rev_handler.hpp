#ifndef REV_HANDLER_HPP
#define REV_HANDLER_HPP

#include <cstdlib>
#include <map>
#include <mongoose/mongoose.h>
#include <stdint.h>
#include <string>
#include <vector>

enum rev_status_t
{
	UNKNOWN,
	PLAINTEXT,
	RECV_PUBKEY,
	CHALLENGE,
	ENCRYPTED
};

struct rev_client_t
{
	std::string address;
	bool alive;
	rev_status_t status;
	int64_t timeout;
	std::string pubkey;
	std::string scratch;
	std::vector<std::string> chunks;
	std::vector<std::string> buffered_cmds;
	std::string secret;
	std::string iv;
};

class rev_handler_t;

typedef std::vector<rev_client_t> rev_client_list_t;
typedef std::map<mg_connection*,rev_client_t> rev_client_map_t;
typedef void(*rev_handler_cb_t)(rev_handler_t&,const rev_client_t&);

class rev_handler_t
{
	public:
		rev_handler_t(rev_handler_cb_t add_cb=NULL,rev_handler_cb_t remove_cb=NULL,
			rev_handler_cb_t recv_cb=NULL);
		~rev_handler_t();
		void connect(const std::string& address);
		void update();
		void add(mg_connection* conn);
		void remove(mg_connection* conn);
		void recv(mg_connection* conn,std::string buffer);
		void send(const std::string& address,std::string buffer);
		void kill(const std::string& address);
		rev_client_list_t list() const;
		const rev_client_map_t& map() const;
	private:
		rev_handler_t(const rev_handler_t& copy);
		rev_handler_t& operator=(const rev_handler_t& copy);
		bool connected_m;
		mg_mgr mgr_m;
		rev_client_map_t clients_m;
		rev_handler_cb_t add_cb_m;
		rev_handler_cb_t remove_cb_m;
		rev_handler_cb_t recv_cb_m;
		std::string private_key_m;
};

#endif