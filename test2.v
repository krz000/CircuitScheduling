module sample(clk, rst, v13.15, v3, v6, j, u78, v);
input clk, rst;
output v13.15;
input v3;
input v6;
input j;
input u78;
input v;

wire v3;
wire v6;
wire j;
wire u78;
wire v;
wire v13.15;

assign v = 1;
assign v13.15 = v3 & !u78 | v6 & u78 | !v3 & j & u78;

endmodule