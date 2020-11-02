/**
 * @file client.hpp
 * @author Hunter Sun (HunterSun2018@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-09-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef VIOLAS_CLIENT
#define VIOLAS_CLIENT

#if __cplusplus < 201703
#error c++ complier must be greater than c++17
#endif

#include <memory>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <limits>
#include <variant>
#include <optional>

namespace violas
{
    const size_t ADDRESS_LENGTH = 16;
    using Address = std::array<uint8_t, ADDRESS_LENGTH>;
    using AuthenticationKey = std::array<uint8_t, ADDRESS_LENGTH * 2>;
    using PublicKey = std::array<uint8_t, ADDRESS_LENGTH * 2>;
    using VecU8 = std::vector<uint8_t>;

    struct AddressAndIndex
    {
        Address address;
        size_t index;
    };

    enum AccountStatus
    {
        Local,
        Persisted,
        Unknow
    };

    struct Account
    {
        uint64_t index;
        Address address;
        AuthenticationKey auth_key;
        PublicKey pub_key;
        uint64_t sequence_number;
        AccountStatus status;
    };

    struct TypeTag
    {
        Address address;
        std::string module_name;
        std::string resource_name;

        TypeTag(Address addr,
                std::string_view mod,
                std::string_view res)
            : address(addr),
              module_name(mod),
              resource_name(res)
        {
        }

        TypeTag(const TypeTag &tag)
            : address(tag.address),
              module_name(tag.module_name),
              resource_name(tag.resource_name)
        {
        }

        TypeTag(TypeTag &&tag)
        {
            address = std::move(tag.address);
            module_name = std::move(tag.module_name);
            resource_name = std::move(tag.resource_name);
        }

        TypeTag &operator=(TypeTag &&tag)
        {
            address = std::move(tag.address);
            module_name = std::move(tag.module_name);
            resource_name = std::move(tag.resource_name);

            return *this;
        }
    };

    const uint64_t MICRO_COIN = 1E+6;
    const uint64_t ASSOCIATION_ID = std::numeric_limits<uint64_t>::max();
    const Address ASSOCIATION_ADDRESS = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x0A, 0x55, 0x0C, 0x18};
    const Address TESTNET_DD_ADDRESS = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0xDD};
    const Address CORE_CODE_ADDRESS = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x01};

    inline TypeTag make_currency_tag(std::string_view currency_code)
    {
        return TypeTag(CORE_CODE_ADDRESS, currency_code, currency_code);
    }

    class Client
    {
    public:
        static std::shared_ptr<Client>
        create(uint8_t chain_id,
               std::string_view url,
               std::string_view mint_key,
               std::string_view mnemonic,
               std::string_view waypoint);

        virtual ~Client(){};

        virtual void
        test_connection() = 0;

        ////////////////////////////////////////////////////////////////
        //  wallet methods
        ////////////////////////////////////////////////////////////////
        virtual AddressAndIndex
        create_next_account(const std::optional<Address> &address = std::nullopt) = 0;

        virtual std::vector<Account>
        get_all_accounts() = 0;

        virtual void
        create_validator_account(std::string_view currency_code,
                                 const AuthenticationKey &auth_key,
                                 std::string_view human_name) = 0;

        virtual void
        mint_for_testnet(std::string_view currency_code,
                         const Address &receiver,
                         uint64_t amount) = 0;
        /**
         * @brief transfer currency
         * 
         * @param sender_account_ref_id the account index of client's wallet 
         * @param receiver_address      the address of receiver
         * @param currency_code         currency code
         * @param amount                the amount of currency
         * @param gas_unit_price        the gas unit price
         * @param max_gas_amount        the max gas amount, default is 1,000,000
         * @param gas_currency_code     the gas currency code. default is 'LBR'
         */
        virtual void
        transfer(size_t sender_account_ref_id,
                 const Address &receiver_address,
                 std::string_view currency_code,
                 uint64_t amount,
                 uint64_t gas_unit_price = 0,
                 uint64_t max_gas_amount = 1000000,
                 std::string_view gas_currency_code = "LBR") = 0;

        ////////////////////////////////////////////////////////////////
        //  management methods
        ////////////////////////////////////////////////////////////////

        //
        //  Allow  the custom scripts
        //  note that calling method needs violas root privilege
        virtual void
        allow_custom_script() = 0;

        //
        //  Allow  to publish custom module
        //  note that calling method needs violas root privilege
        virtual void
        allow_publishing_module(bool enabled) = 0;

        //
        //  publish a module file
        //  if account_index is ASSOCIATION_ID then publish module with association root account
        virtual void
        publish_module(size_t account_index,
                       std::string_view module_file_name) = 0;

        using TransactionAugment = std::variant<
            uint8_t,
            uint64_t,
            __uint128_t,
            Address,
            std::vector<uint8_t>,
            bool>;

        /**
         * @brief Execute script file with arguments
         * 
         * @param account_index account index of wallet
         * @param script_file_name script file name with path
         * @param type_tags transaction TypeTag vector for script
         * @param arguments transaction argument vector for script 
         */
        virtual void
        execute_script_file(size_t account_index,
                            std::string_view script_file_name,
                            const std::vector<TypeTag> &type_tags = {},
                            const std::vector<TransactionAugment> &arguments = {},
                            bool is_blocking = true) = 0;

        /**
         * @brief Execute script bytecode with arguments
         * 
         * @param account_index account index of wallet
         * @param script_file_name script file name with path
         * @param type_tags transaction TypeTag vector for script
         * @param arguments transaction argument vector for script 
         */
        virtual void
        execute_script(size_t account_index,
                       const std::vector<uint8_t> &script,
                       const std::vector<TypeTag> &type_tags = {},
                       const std::vector<TransactionAugment> &arguments = {},
                       bool is_blocking = true) = 0;

        /**
         * @brief Query accout status infomation
         * 
         * @param address - the address of account
         * @return std::string 
         */
        virtual std::string
        query_account_info(const Address &address) = 0;

        /**
         * @brief Query transaction inforamtion by address and seqence number
         * 
         * @param address the address of account
         * @param seq_number    the sequence number of account
         * @param is_fetching_event whether fectching event or not
         * @return std::string with json format
         */
        virtual std::string
        query_transaction_info(const Address &address,
                               uint64_t sequence_number,
                               bool is_fetching_events) = 0;
        /**
         * @brief Query transaction inforamtion by range
         * 
         * @param start_version     start version 
         * @param limit             limit of range, amount of queried transaction
         * @param is_fetching_event whether fectching event or not
         * @return std::string  with json format
         */
        virtual std::string
        query_transaction_info(uint64_t start_version,
                               uint64_t limit,
                               bool is_fetching_events) = 0;
        /**
         * @brief evnet type
         * 
         */
        enum event_type
        {
            sent,
            received
        };

        /**
         * @brief Query events
         * 
         * @param address       the address of account
         * @param type          evnet type
         * @param start_version start version
         * @param limit         limit of rang, amount of queried events
         * @return std::string  with json format
         */
        virtual std::string
        query_events(const Address &address,
                     event_type type,
                     uint64_t start_version,
                     uint64_t limit) = 0;

        //
        //  Execute script file with specified arguments
        //
        // template <typename... Args>
        // void execute_script(size_t account_index,
        //                     std::string_view script_file_name,
        //                     const std::vector<TypeTag> &type_tags,
        //                     const Args &... args)
        // {
        //     using namespace std;

        //     std::vector<TransactionAugment> txn_args;
        //     auto parse_arg = [&](const auto &arg) {
        //         txn_args.push_back(arg);
        //     };

        //     ((parse_arg(args)), ...);
        //     //(args.push_back(std::forward<TransactionAugment>(args)), ...);

        //     execute_script(account_index, script_file_name, type_tags, txn_args);
        // }

        ///////////////////////////////////////////////////////
        // multi-currency method
        ///////////////////////////////////////////////////////

        // Call this method with root association privilege
        virtual void publish_curency(std::string_view currency_code) = 0;

        // Register currency with association root account
        virtual void
        register_currency(std::string_view currency_code,
                          uint64_t exchange_rate_denom,
                          uint64_t exchange_rate_num,
                          bool is_synthetic,
                          uint64_t scaling_factor,
                          uint64_t fractional_part) = 0;

        /// add currency for the designated dealer account
        virtual void
        add_currency_for_designated_dealer(
            std::string_view currency_code,
            const Address &dd_address) = 0;

        /// Add a currency to current account
        virtual void
        add_currency(size_t account_index,
                     std::string_view currency_code) = 0;

        /// get the balance of currency for the account address
        virtual uint64_t
        get_currency_balance(const Address &address,
                             std::string_view currency_code) = 0;

        /// Get all currency info
        virtual std::string
        get_all_currency_info() = 0;

        /// mint currency for dd account
        virtual void
        mint_currency_for_designated_dealer(std::string_view currency_code,
                                            uint64_t sliding_nonce,
                                            const Address &dd_address,
                                            uint64_t amount,
                                            uint64_t tier_index) = 0;

        virtual void
        create_designated_dealer_account(std::string_view currency_code,
                                         uint64_t nonce,
                                         const Address &new_account_address,
                                         const AuthenticationKey &auth_key,
                                         std::string_view human_name,
                                         std::string_view base_url,
                                         PublicKey compliance_public_key,
                                         bool add_all_currencies) = 0;

        virtual void
        update_account_authentication_key(const Address &address,
                                          const AuthenticationKey &auth_key) = 0;

        //
        // Create parent VASP account
        //
        virtual void
        create_parent_vasp_account(std::string_view currency_code,
                                   uint64_t nonce,
                                   const Address &new_account_address,
                                   const AuthenticationKey &auth_key,
                                   std::string_view human_name,
                                   std::string_view base_url,
                                   PublicKey compliance_public_key,
                                   bool add_all_currencies) = 0;

        //  Exchnage interface
        //
        virtual std::string
        get_exchange_currencies(const Address &address) = 0;

        //
        //  get exchange reservers
        //
        virtual std::string
        get_exchange_reserves(const Address &address) = 0;

        //
        //  get liquidity balance
        //
        virtual std::string
        get_liquidity_balance(const Address &address) = 0;
    };

    using client_ptr = std::shared_ptr<Client>;
} //namespace violas

#endif