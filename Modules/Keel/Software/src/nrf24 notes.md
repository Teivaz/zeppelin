Always check if the packet width reported is 32 bytes or shorter when using the `R_RX_PL_WID` command. If its width is longer than 32 bytes then the packet contains errors and must be discarded. Discard the packet by using the `Flush_RX` command.


In order to enable DPL the `EN_DPL` bit in the `FEATURE` register must be enabled. In RX mode the DYNPD register must be set. A PTX that transmits to a PRX with DPL enabled must have the `DPL_P0` bit in `DYNPD` set.

To enable Enhanced ShockBurst set EN_AA and ARC

The CRC is the mandatory error detection mechanism in the packet

For 250kbps data rate ARD >= 500μs


TX:
Enable TX mode
Write data in TX FIFO with `W_TX_PAYLOAD`
Set CE high

Wait for `MAX_RT` or `TX_DS`
Set CE low
If `MAX_RT` is set the tx pipe needs to be flushed with `Flush_TX`


RX:
Enable RX mode
Set CE high

Wait for `RX_DR`
Read payload length `RX_PW_Px`
Read payload with `R_RX_PAYLOAD`

