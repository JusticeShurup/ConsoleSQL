#include <iostream>
#include <sstream>
#include <string>
#include <Poco/Data/Session.h>
#include <Poco/Data/PostgreSQL/Connector.h>


using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
typedef Poco::Tuple<int, std::string, std::string, int> User;
void sort(std::vector<User> users) {
    for(int i = 0; i < users.size() - 1; i++)
        for (int j = 0; j < users.size() - 1; j++) 
            if (users[j].get<0>() < users[j + 1].get<0>()) {
                User user = users[j];
                users[j] = users[j + 1];
                users[j + 1] = user;
            }
}
void listUsers(std::vector<User> users, Session session) {
    Statement getUsers(session);
    getUsers << "SELECT * FROM Users",
        into(users),
        now;
    std::cout << "List of users: " << std::endl;
    for (auto& user : users)
        std::cout << user.get<0>() << " " << user.get<1>() << std::endl;

}/*
class User {
public:
    User(int id, std::string login, std::string password, int is_admin) {
        this->id = id;
        this->login = login;
        this->password = password;
        this->is_admin = is_admin;
    };
    void getInfo() {
        std::cout << id << " " << login;
    }   
    void getFullInfo() {
        std::cout << id << " " << login << " " << password << " " << is_admin;
    }


private:
    int id;
    std::string login;
    std::string password;
    int is_admin;

};

class Orders {
public:
    Orders() {};
    findUser(int id) {

    }
    void editUser(int id) {
    }
private: 
    User user;

};*/
int main()
{
    Poco::Data::PostgreSQL::Connector::registerConnector();
    std::string username = "playground";
    std::string password = "playground";
    std::stringstream ss;
    ss << "host=192.168.56.101 port=5432 dbname=Playground user="
        << username
        << " password="
        << password;
    Session session("PostgreSQL", ss.str());
    int id = 1;
    int is_admin;
    std::string login;
    std::string pas;
    Statement stmt(session);
    std::vector<User> users;
    //1 задание stmt << "CREATE TABLE users (id integer PRIMARY KEY, login varchar(15), password varchar(15), is_admin smallint)", now;

    /* 2 задание
    stmt << "INSERT INTO users(id, login, password, is_admin) VALUES($1, $2, $3, $4)", 
        use(id),
        use(login),
        use(pas),
        use(is_admin);

    for (int i = 1; i < 4; i++) {
        id = i;
        std::cout << "Enter login: " << std::endl;
        std::cin >> login;
        std::cout << "Enter password: " << std::endl;
        std::cin >> pas;
        std::cout << "Is user admin? Write yes/no: " << std::endl;
        std::string choice;
        std::cin >> choice;
        if (choice == "yes")
            is_admin = 1;
        else
            is_admin = 0;
        stmt.execute();
    }
    */
    stmt << "SELECT id, is_admin FROM Users WHERE login=$1 AND password=$2",
        use(login),
        use(pas),
        into(id),
        into(is_admin),
        range(0, 1);
    while (true) {
        std::cout << "Enter your login: ";
        std::cin >> login;
        std::cout << "Enter password: ";
        std::cin >> pas;
        stmt.execute();
        if (!stmt.execute()) {
            std::cout << "Login failed" << std::endl;
            std::cout << "try again" << std::endl;
        }
        else {
            break;
        }
    }
    if (is_admin == 1) {
        Statement getUsers(session);
        std::vector<User> users;
        getUsers << "SELECT * FROM Users",
            into(users),
            now;
        listUsers(users, session);
        int choice;
        while (true) {
            std::cout << "Choose what you want to do:\n 1. Edit user \n 2. Delete user \n 3. Add new user \n 4.Exit" << std::endl;
            std::cin >> choice;
            Statement findUser(session);
            findUser << "SELECT login, password, is_admin FROM Users WHERE id = $1",
                use(id),
                into(login),
                into(pas),
                into(is_admin),
                range(0, 1);
            if (choice == 0) {
                std::cout << "Choose what you want to do:\n 1. Edit user \n 2. Delete user \n 3. Add new user \n 4.Exit" << std::endl;
                std::cin >> choice;
            }
            else if(choice == 1)
            {
                std::cout << "Choose id of user" << std::endl;
                listUsers(users, session);
                std::cin >> id;
                if (!findUser.execute())
                    std::cout << "Incorrect id" << std::endl;
                else {
                    findUser.execute();
                    std::cout << "Enter new or old: login, password, is_admin" << std::endl;
                    std::cin >> login; std::cin >> pas; std::cin >> is_admin;
                    Statement editUser(session);
                    editUser << "UPDATE Users SET login = $2, password = $3, is_admin = $4 WHERE id = $1",
                        use(id),
                        use(login),
                        use(pas),
                        use(is_admin),
                        now;
                    listUsers(users, session);
                }
                choice = 0;
            }
            else if(choice == 2) 
            {
                std::cout << "Choose id of user" << std::endl;
                std::cin >> id;
                Statement deleteUser(session);
                deleteUser << "DELETE FROM Users WHERE id =$1",
                    use(id),
                    now;
                choice = 0;
            }
            else if(choice == 3){
                Statement addUser(session);
                std::cout << "Enter id, login, password, is_admin" << std::endl;
                std::cin >> id;
                std::cin >> login;
                std::cin >> pas;
                std::cin >> is_admin;
                addUser << "INSERT INTO Users(id, login, password, is_admin) VALUES($1, $2, $3, $4)",
                    use(id),
                    use(login),
                    use(pas),
                    use(is_admin);
                addUser.execute();
                choice = 0;
            }
            else{
                std::cout << "Have a nice day" << std::endl;
                break;
            }
        }
    }
    else
        std::cout << "Logged successfully" << std::endl;
    return 0;
}
