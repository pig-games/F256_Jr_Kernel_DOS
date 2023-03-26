            .cpu    "65c02"

runfl       .namespace

            .section    code
cmd
            lda     readline.tokens+1
            sta     kernel.args.buf+0
            lda     #>readline.buf
            sta     kernel.args.buf+1
            lda     #1
            jsr     readline.token_length
            tay
            lda     #0
            sta     (kernel.args.buf),y
            jsr     kernel.RunNamed
            .send
            .endn
