module sample03(clk, rst, o, p, a, b, c, d, e);

input clk, rst;

output o;
output p;

input a;
input b;
input c;
input d;
input e;

wire a;
wire b;
wire c;
wire d;
wire e;
wire o;
wire p;
wire f;
wire g;
wire h;
wire i;
wire j;
wire k;
wire l;

assign f =  a  |  b ;
assign g =  b & d ;
assign h =  f & !g  |  !f & g ;
assign i =  c  |  h ;
assign j =  e  |  f  |  g ;
assign k =  i & j ;
assign l =  !d & e ;
assign o =  !l ;
assign p =  !k ;

endmodule
