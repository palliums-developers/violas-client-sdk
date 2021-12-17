#pragma once
#include <optional>
#include <violas_client2.hpp>
#include <bcs_serde.hpp>

namespace nft
{
    using Id = std::array<uint8_t, 32>;
    using Address = std::array<uint8_t, 16>;

    struct Order
    {
        bytes nft_token_id;
        uint64_t price;
        bytes currency_code;
        uint64_t sale_incentive;
        Address provider;

        BcsSerde &serde(BcsSerde &bs)
        {
            return bs && nft_token_id && price && currency_code && sale_incentive && provider;
        }
    };

    struct MadeOrderEvent
    {
        Id order_id;
        Id nft_token_id;
        uint64_t price;
        bytes currency_code;
        uint64_t sale_incentive;

        BcsSerde &serde(BcsSerde &bs)
        {
            return bs && order_id && nft_token_id && price && currency_code && sale_incentive;
        }
    };

    struct AccountInfo
    {
        BcsSerde &serde(BcsSerde &bs)
        {
            return bs;
        }
    };

    class Store
    {
    private:
        /* data */
        violas::client2_ptr _client;
        diem_types::TypeTag _nft_type_tag;

    public:
        Store(violas::client2_ptr client, const diem_types::TypeTag &nft);

        ~Store();

        void initialize();

        void register_nft();

        void register_account(size_t account_index);

        void make_order(size_t account_index,
                        Id nft_token_id,
                        uint64_t price,
                        std::string_view currency,
                        double incentive);

        void revoke_order(size_t account_index,
                          Id order_id);

        void trade_order();

        std::optional<AccountInfo>
        get_account_info(Address address);

        std::vector<Order>
        list_orders();

        std::vector<MadeOrderEvent>
        get_made_order_events(Address address, uint64_t start, uint64_t limit);
    };

} // namespace nft

std::ostream &operator<<(std::ostream &os, const std::vector<nft::Order> &orders);
