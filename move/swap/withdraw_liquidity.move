script {
    use 0x1::Exchange;

    fun withdraw_liquidity<Token1: store, Token2: store>(
        sender : signer,
        amount : u64 ) {
        Exchange::withdraw_liquidity<Token1, Token2>(&sender, amount);
    }
}