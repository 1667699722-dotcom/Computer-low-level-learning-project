//`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//                                                                              //
//  Author: Liuhuachao                                                          //
//          Liuhuachaohaha@gmail.com                                            //                          //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

module uart_test(
	input                        clk,
	input                        rst_n,
	input                        uart_rx,
	output                       uart_tx,
	input [3:0]           key,       //input four key signal,when the keydown,the value is 0
	output[3:0]           led    //LED display ,when the siganl high,LED lighten
	);
wire [7:0] result;

reg [7:0] register_file [0:7];
reg [2:0] bit_index;

reg [63:0] register_file_rec;
reg [2:0] bit_index_rec;

reg[3:0] led_r;           //define the first stage register , generate four D Flip-flop 
reg[3:0] led_r1;          //define the second stage register ,generate four D Flip-flop

parameter                        CLK_FRE = 50;//Mhz
reg[7:0]                         tx_data;
reg                              tx_data_valid;
wire                             tx_data_ready;
wire[7:0]                        rx_data;
wire                             rx_data_valid;
reg                             rx_data_ready;

wire key_press=led_r[0] & ~led_r1[0];
reg [2:0] state;
parameter IDLE       = 3'd0;  
parameter DETECT     = 3'd1;  
parameter SEND       = 3'd2; 
parameter WAIT_TX    = 3'd3;  
parameter WAIT_RELEASE = 3'd4; 

reg [2:0] statec;
parameter IDLEC       = 3'd0;  
parameter REC    = 3'd1;  
parameter WAIT_RX       = 3'd2; 

always@(posedge clk or negedge rst_n)
begin
   if(!rst_n)
	  begin
	  bit_index_rec<=3'd0;
	  rx_data_ready<=1'd0;
	  register_file_rec[0]<=8'd0;
	  register_file_rec[1]<=8'd0;
	  register_file_rec[2]<=8'd0;
	  register_file_rec[3]<=8'd0;
	  register_file_rec[4]<=8'd0;
	  register_file_rec[5]<=8'd0;
	  register_file_rec[6]<=8'd0;
	  register_file_rec[7]<=8'd0;
	  end
	else
	  begin
	     case(statec)
		      IDLEC:
				begin
				if(rx_data_valid)
				   begin
					  register_file_rec<=(register_file_rec&~(8'b11111111<<(8*bit_index_rec)))|(rx_data<<(8*bit_index_rec));
					  statec<=REC;
					end
				end
				REC:
				begin
					statec <= WAIT_RX;
					rx_data_ready<=1'd1;
				end
				WAIT_RX:
				begin
				   rx_data_ready<=1'd0;
					if(bit_index_rec<3'd7)
						  begin
						     bit_index_rec<=bit_index_rec+1'b1;
						  end
					else
						  begin
						     bit_index_rec<=3'b0;
						  end
					statec <= IDLEC;
				end
		  endcase
	  end
end
//
always@(posedge clk or negedge rst_n)
begin
	if(!rst_n)
		begin
			state <= IDLE;
			tx_data_valid <= 1'b0;
			tx_data <= 8'd0;
			bit_index<=3'd0;
			
		end
	else
		begin
			case(state)
				IDLE:
					begin
						tx_data_valid <= 1'b0;
						if(key_press) 
							begin
								state <= DETECT;
								tx_data <= register_file[bit_index];  
							end
					end
				DETECT:
					begin
						state <= SEND;
						tx_data_valid <= 1'b1;
					end
				SEND:
					begin
						state <= WAIT_TX;
						tx_data_valid <= 1'b0;  
					end
				WAIT_TX:
					begin
						state <= WAIT_RELEASE;
						if(bit_index<3'd7)
						  begin
						     bit_index<=bit_index+1'b1;
						  end
						else
						  begin
						     bit_index<=3'b0;
						  end
					end
				WAIT_RELEASE:
					begin
						if(led_r1[0] == 1'b0) 
							begin
								state <= IDLE;
							end
					end
				default:
					begin
						state <= IDLE;
					end
			endcase
		end
end

always@(posedge clk or negedge rst_n)
	begin
		if(!rst_n)
			begin
         register_file[0]<=8'd1;
			register_file[1]<=8'd1;
			register_file[2]<=8'd1;
			register_file[3]<=8'd1;
			register_file[4]<=8'd1;
			register_file[5]<=8'd1;
			register_file[6]<=8'd1;
			register_file[7]<=8'd1;
			end
		else
			begin
			register_file[0]<=result;
			end
		end
		
always@(posedge clk)
begin
	led_r <= ~key;        //first stage latched data
end

always@(posedge clk)
begin
	led_r1 <= led_r;      //second stage latched data
end

assign led = led_r1;


uart_rx#
(
	.CLK_FRE(CLK_FRE),
	.BAUD_RATE(115200)
) uart_rx_inst
(
	.clk                        (clk                      ),
	.rst_n                      (rst_n                    ),
	.rx_data                    (rx_data                  ),
	.rx_data_valid              (rx_data_valid            ),
	.rx_data_ready              (rx_data_ready            ),
	.rx_pin                     (uart_rx                  )
);

uart_tx#
(
	.CLK_FRE(CLK_FRE),
	.BAUD_RATE(115200)
) uart_tx_inst
(
	.clk                        (clk                      ),
	.rst_n                      (rst_n                    ),
	.tx_data                    (tx_data                  ),
	.tx_data_valid              (tx_data_valid            ),
	.tx_data_ready              (tx_data_ready            ),
	.tx_pin                     (uart_tx                  )
);

adder#
(
	.CLK_FRE(CLK_FRE),
	.BAUD_RATE(115200)
) u_adder
(
	.clk                        (clk                      ),
	.rst_n                      (rst_n                    ),
	.in_register            (register_file_rec),
	.out_register               (result)
);

endmodule