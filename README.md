# MIR-Engine
MIR-Engine


[CODE]push ebp                        enter Imm, 0
mov ebp, esp               ->   nop
add esp, -Imm                   nop
 
push ebp                        enter -Imm, 0
mov ebp, esp               ->   nop
add esp, Imm                    nop
 
mov esp, ebp               ->   leave
pop ebp                         nop
 
mov Reg1, Reg2             ->   push Reg1  /  pop Reg2
 
xor Reg, Reg               ->   sub Reg, Reg
sub Reg, Reg               ->   xor Reg, Reg
 
or Reg, Reg                ->   test Reg, Reg
and Reg, Reg               ->   or Reg, Reg
test Reg, Reg              ->   and Reg, Reg
 
add Reg, Imm              <->   sub Reg, -Imm
sub Reg, Imm              <->   add Reg, -Imm
 
mov Reg, fs:[0]            ->   xor Reg, Reg  /  mov Reg, fs:[Reg]
 
mov EAX, fs:[0]            ->   xor EAX, EAX  /  mov EAX, fs:[EAX]
 
xor Reg, Reg  /  dec Reg   ->   or Reg, -1
 
xor/sub Reg, Reg  /  mov Reg_8, Reg1_8  ->   movzx Reg, Reg1_8
 
Inverse All (ROL,ROR,RCL,RCR,SHL/SHA,SHR,SAR,ADD,SUB,AND,OR,XOR,MOV,CMP Reg1, Reg2)
 
It also contains a shrinker of instructions, for example add eax, 10 followed by sub eax, 20 will become add eax, -10 or mov Reg, 10 followed by add Reg, 10 will become mov eax, 20.
This is not really useful for PE files since they are already optimized.
[\CODE]
