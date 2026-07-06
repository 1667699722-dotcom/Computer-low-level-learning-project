module adder
#(
	parameter CLK_FRE = 50,      //clock frequency(Mhz)
	parameter BAUD_RATE = 115200 //serial baud rate
)
(
	input                        clk,              //clock input
	input                        rst_n,            //asynchronous reset input, low active 
    input   [63:0] in_register ,
    output reg [7:0] out_register
);

always @(posedge clk or negedge rst_n)
begin
    if(!rst_n)
    begin
        out_register<=8'd0;
    end
    else
    begin
        out_register<=(in_register>>(8*0) & 8'b11111111)+((in_register>>(8*1)) & 8'b11111111)
		  +(in_register>>(8*2) & 8'b11111111)+((in_register>>(8*3)) & 8'b11111111)
		  +(in_register>>(8*4) & 8'b11111111)+((in_register>>(8*5)) & 8'b11111111)
		  +(in_register>>(8*6) & 8'b11111111)+((in_register>>(8*7)) & 8'b11111111)
		  ;
    end
end
endmodule