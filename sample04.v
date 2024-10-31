module sample04(clk, rst, r, s, t, x, y, z, u, v, w);

input clk, rst;

output r;
output s;
output t;

input x;
input y;
input z;
input u;
input v;
input w;

wire x;
wire y;
wire z;
wire u;
wire v;
wire w;
wire r;
wire s;
wire t;
wire n;
wire p;
wire q;
wire m;

assign n =  x & y & z ;
assign p =  n  |  u ;
assign q =  p  |  w ;
assign m =  v  |  x  |  y ;
assign r =  q & m ;
assign s =  u & w ;
assign t =  !z ;

endmodule
