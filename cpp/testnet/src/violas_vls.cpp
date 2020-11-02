#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <functional>
#include <client.hpp>
#include "utils.h"

using namespace std;
using namespace violas;

void initialize_timestamp(client_ptr client);
void mine_vls(client_ptr client);

const tuple<Address, string> VLS_ADDRESSES[] =
    // 0000000000000000000000000000DD00
    {{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xDD, 0x00}, "garbage"},
     {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xDD, 0x01}, "community"},
     {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xDD, 0x02}, "investor"},
     {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xDD, 0x03}, "association"},
     {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xDD, 0x04}, "dev-team"},
     {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xDD, 0x05}, "consultant"},
     {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xDD, 0x06}, "contributor"}};

int main(int argc, const char *argv[])
{
    try
    {
        if (argc < 6)
            throw runtime_error("missing arguments. \n Usage : url mint_key mnemonic waypoint chain_id");

        uint8_t chain_id = stoi(argv[5]);
        string url = argv[1];
        string mint_key = argv[2];
        string mnemonic = argv[3];
        string waypoint = argv[4];

        auto client = Client::create(chain_id, url, mint_key, mnemonic, waypoint);

        client->test_connection();

        using handler = function<void()>;
        map<int, handler> handlers = {
            {1, [=]() { mine_vls(client); }},
            {2, [=]() { initialize_timestamp(client); }},
        };

        cout << "1 for distribute vls \n"
                "2 for initialize vls timestamp \n"                
                "Please input index : ";

        int index;
        cin >> index;

        handlers[index]();
    }
    catch (const std::exception &e)
    {
        std::cerr << color::RED
                  << "caught an exception : " << e.what()
                  << color::RESET
                  << endl;
    }

    return 0;
}

void mine_vls(client_ptr client)
{
    vector<uint8_t> mint_vls_bytecode = {
        161, 28, 235, 11, 1, 0, 0, 0, 5, 1, 0, 2, 3, 2, 5, 5, 7, 1, 7, 8, 22, 8, 30,
        16, 0, 0, 0, 1, 0, 0, 0, 0, 12, 76, 105, 98, 114, 97, 65, 99, 99, 111, 117,
        110, 116, 8, 109, 105, 110, 101, 95, 118, 108, 115, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 17, 0, 2};

    //string
    auto [address0, name] = VLS_ADDRESSES[0];
    client->create_next_account(address0);

    auto accounts = client->get_all_accounts();

    client->execute_script(0, mint_vls_bytecode);

    cout << color::GREEN << "succeeded to mine VLS." << color::RESET << endl;
}

void initialize_timestamp(client_ptr client)
{
    client->allow_publishing_module(true);
    client->allow_custom_script();

    cout << "allow custom script and  publishing module." << endl;

    vector<uint8_t> vls_initialize_timestamp = {
        161, 28, 235, 11, 1, 0, 0, 0, 5, 1, 0, 2, 3, 2, 5, 5, 7, 1, 7, 8, 25, 8, 33,
        16, 0, 0, 0, 1, 0, 0, 0, 0, 3, 86, 76, 83, 20, 105, 110, 105, 116, 105, 97, 108,
        105, 122, 101, 95, 116, 105, 109, 101, 115, 116, 97, 109, 112, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 17, 0, 2};

    for (const auto address : VLS_ADDRESSES)
    {
        client->create_next_account(get<0>(address));
    }

    auto accounts = client->get_all_accounts();

    try_catch([&]() {
        for (auto account : accounts)
        {
            client->create_designated_dealer_account("Coin1", 0,
                                                     account.address, account.auth_key,
                                                     get<1>(VLS_ADDRESSES[account.index]),
                                                     "wwww.violas.io",
                                                     account.pub_key, true);

            client->update_account_authentication_key(account.address, account.auth_key);

            cout << "address : " << account.address
                 << ", auth key : " << account.auth_key << endl;
        }

        cout << color::GREEN << "Created all accounts for VLS receivers." << color::RESET << endl;
    });

    client->execute_script(0, vls_initialize_timestamp);

    cout << color::GREEN << "Initialized timestamp for VLS module." << color::RESET << endl;
}