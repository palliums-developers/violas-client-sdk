script {
    use 0x2::NonFungibleToken;
    
    fun nft_accept<T: store>(sig: signer) {
        
        NonFungibleToken::accept<T>(&sig);
    }

}