# Relay net

TWELITE NET has a LayerTree network as a relay network.

A LayerTree network is a relay network designed for efficient upstream delivery. LayerTree_mininodes, which are solely for transmission, are also part of the LayerTree network.

## Basic principles

* There is only one parent unit and all communication is centralised in the parent unit.
* As a general rule, the direction of delivery is from the end node to the upper level and finally to the parent machine.
  * Downward delivery from the parent unit is by broadcast communication.
  * Communication between sub-units and repeaters is only possible after delivery to the parent unit.
  * However, it is possible to send directly between nodes that are within radio range by specifying their addresses.
* The transponder predetermines its own number of levels.
* Connect to a repeater within radio range that is higher in the hierarchy than your own (the repeater with the lower hierarchy number).
* By using LayerTree Mininodes, which do not have a destination repeater but only transmit, they can act as sensor nodes with very low battery consumption. (App_Tag)

![](<../../../.gitbook/assets/image (51).png>)

(RED) : Parent node

(YELLOW `n`) : Child node which can relay. (`n` represents number of layer)

(Blue) : Child node which can work with sleeping.

## About layers

The parent machine is numbered 0 and can number its layers up to 63.

![](<../../../.gitbook/assets/image (52).png>)

It is OK to skip numbers as long as they are in ascending order.
