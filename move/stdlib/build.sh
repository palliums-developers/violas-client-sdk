rm modules/*.mv scripts/*.mv

echo "build allow_publishing_module.move"
../build.sh scripts/allow_publishing_module.move
mv build/scripts/main.mv scripts/allow_publishing_module.mv

echo "build allow_custom_script.move"
../build.sh scripts/allow_custom_script.move
mv build/scripts/main.mv scripts/allow_custom_script.mv

echo "build create_designated_dealer_ex.move"
../build.sh scripts/create_designated_dealer_ex.move
mv build/scripts/create_designated_dealer_ex.mv scripts/

echo "build modules/register_stable_currency.move"
../build.sh scripts/register_stable_currency.move
mv build/scripts/register_stable_currency.mv scripts/

# echo "build distribute_vls_from_community.move"
../build.sh scripts/distribute_vls_from_community.move ../bank/bank.move ../oracle/oracle.move ../exchange/exchange.move
mv build/scripts/distribute_vls_from_community.mv scripts/

echo "build add_currency_for_designated_dealer.move"
../build.sh scripts/add_currency_for_designated_dealer.move
mv build/scripts/add_currency_for_designated_dealer.mv scripts/
mv build/scripts/dd_add_currency.mv scripts/

echo "build vls_initialize_timestamp.move"
../build.sh scripts/vls_initialize_timestamp.move
mv build/scripts/main.mv scripts/vls_initialize_timestamp.mv

echo "build mine_vls.move"
../build.sh scripts/mine_vls.move
mv build/scripts/main.mv scripts/mine_vls.mv

echo "build recover_vls_fees_to_association.move"
../build.sh scripts/recover_vls_fees_to_association.move
mv build/scripts/recover_vls_fees_to_association.mv scripts/


# echo "build SortedLinkedList.move"
# ../build.sh remove_liquidity.move 0x0 exchange.move exdep.move
# mv build/scripts/main.mv remove_liquidity.mv

echo "build modules/Compare.move"
../build.sh modules/Compare.move
mv build/modules/0_Compare.mv modules/Compare.mv

echo "build modules/Set.move"
../build.sh modules/Set.move modules/Compare.move
mv build/modules/0_Set.mv modules/Set.mv

echo "build modules/Map.move"
../build.sh modules/Map.move modules/Compare.move
mv build/modules/0_Map.mv modules/Map.mv 
#
#   Building NFT module and scripts
#
echo "build modules/NonFungibleToken.move"
../build.sh modules/NonFungibleToken.move modules/Compare.move modules/SimpleSortedLinkedList.move modules/SortedLinkedList.move modules/Map.move
mv build/modules/0_NonFungibleToken.mv modules/NonFungibleToken.mv

echo "build scripts/nft_register.move"
../build.sh scripts/nft_register.move modules/NonFungibleToken.move modules/Map.move modules/Compare.move
mv build/scripts/nft_register.mv scripts/

echo "build scripts/nft_accept.move"
../build.sh scripts/nft_accept.move modules/NonFungibleToken.move modules/Map.move modules/Compare.move
mv build/scripts/nft_accept.mv scripts/

echo "build scripts/nft_burn.move"
../build.sh scripts/nft_burn.move modules/NonFungibleToken.move modules/Map.move modules/Compare.move
mv build/scripts/nft_burn.mv scripts/

echo "build scripts/nft_transfer.move"
../build.sh scripts/nft_transfer.move modules/NonFungibleToken.move modules/Map.move modules/Compare.move
mv build/scripts/nft_transfer_by_token_id.mv scripts/
mv build/scripts/nft_transfer_by_token_index.mv scripts/

echo "build modules/NftStore.move"
../build.sh modules/NftStore.move modules/NonFungibleToken.move modules/Map.move modules/Compare.move
mv build/modules/0_NftStore.mv modules/NftStore.mv

echo "build scripts/nft_store_scripts.move"
../build.sh scripts/nft_store_scripts.move modules/NonFungibleToken.move modules/NftStore.move modules/Map.move modules/Compare.move
mv build/scripts/nft_store_initialize.mv scripts/
mv build/scripts/nft_store_register_nft.mv scripts/
mv build/scripts/nft_store_accept.mv scripts/
mv build/scripts/nft_store_make_order.mv scripts/
mv build/scripts/nft_store_revoke_order.mv scripts/


rm -rf build