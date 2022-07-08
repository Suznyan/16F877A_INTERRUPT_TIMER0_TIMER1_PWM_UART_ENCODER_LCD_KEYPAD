
void UART_init(void)
{
    //Setting I/o pins for UART
    TRISC6 = 0; //TX pin as output
    TRISC7 = 1; //RX pin as input
    
    //Initialize SPBRG register for required
    //baud rate and set BRGH for fast baud_rate
    SPBRG = ((_XTAL_FREQ/16)/Baud_rate) - 1;
    BRGH = 1; //for high baud_rate
    
    //Enable Asynchronous serial port
    SYNC = 0; //Asynchronous
    SPEN = 1; //Enable serial port pins
    
    //Lets prepare for transmission & reception
    TXEN = 1; //Enable transmission
    CREN = 1; //Enable reception
    
    //Select 8-bit mode
    TX9 = 0; //8-bit reception selected
    RX9 = 0; //8=bit reception mode selected
}

//Function to send one byte of date to UART
void UART_send_char(char bt)
{
    while(!TXIF); //hold the program till TX buffer is free
    TXREG = bt; //load the transmitter buffer with the received value
}

//Function to convert string to byte
void UART_send_string(char* st_pt)
{
    while(*st_pt) //if there is a char
        UART_send_char(*st_pt++); //process it as byte data
}

//Function to get one byte of date from UART
char UART_get_char()
{
    if(OERR) //check for error
    {
        CREN = 0; //If error -> reset
        CREN = 1; //If error -> reset
    }
    
    while(!RCIF); // hold the program till RX buffer is free
    
    return RCREG; // receive the value and send it to main function
}