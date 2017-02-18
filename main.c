#include <stdio.h>
#include <stdlib.h>


#define MOVE 0
#define ADD 1
#define SUB 2
#define BRA 3
#define CMP 4
#define BEQ 5
#define BNE 6
#define EXG 7
#define ADDSQUARE 8
#define SWAP 9
#define EXGMEM 10
#define STOP 15


void main(void)
{
    unsigned short int memory[256];

    unsigned short int PC = 0;      /* Program counter                  */
    unsigned short int D0 = 1;      /* Data register                    */
    unsigned short int A0 = 2;      /* Address register                 */
    unsigned short int CCR = 0;     /* Condition code register          */
    unsigned short int MAR;         /* Memory address register          */
    unsigned short int MBR;         /* Memory buffer register           */
    unsigned short int IR;          /* Instruction register             */
    unsigned short int operand;     /* The 8-bit operand from IR        */
    unsigned short int operand2;
    unsigned short int source,source2;      /* Source operand                   */
    unsigned short int destination,destination2; /* Destination value                */
    unsigned short int opcode;      /* 4 bit op-code from IR            */
    unsigned short int amode;       /* 2 bit addressing mode            */
    unsigned short int direction;   /* 1 bit data direction flag        */
    unsigned short int run = 1;     /* Execute program while run is 1   */

    unsigned short int counter =0;
    memory[32]=12;
    memory[54]=30;


    while(counter<128)
    {
        memory[counter]=0;
        int opflag=0,dirflag=0,admodeflag=0;

        printf("1)Move\n2)Add\n3)Sub\n4)Branch\n5)Compare\n6)Branch if equal\n7)BNG\n8)EXG\n9)addsquare\n10)Swap\n11)Exchange memory\n16)Stop\n");

        scanf("%d",&opflag);



        memory[counter]=opflag-1;
        memory[counter]=memory[counter]<<4;



        if(opflag==16)
        {
            memory[++counter]=0;
            break;
        }


        printf("\n1)to memory\n2)to register\n");

        scanf("%d",&dirflag);
        printf("1)absolute\n2)literal\n3)indexed\n4)PC relative\n");
        scanf("%d",&admodeflag);

        unsigned short int temp = 0;
        temp = dirflag-1;
        temp = temp<<2;
        temp+=(admodeflag-1);
        memory[counter]+=temp;


        counter++;


        printf("\nEnter value.\n");
        scanf("%d",&memory[counter++]);
        if(opflag-1==10)
        {
            printf("\nEnter value.\n");
            scanf("%d",&memory[counter++]);
        }
        int i ;



    }

    int i;

    while(run)
    {
        int temp =0;
        MAR = PC;           /* PC to MAR                */
        PC = PC + 1;        /* Increment PC             */
        MBR = memory[MAR];  /* Get next instruction     */
        IR = MBR;           /* Copy MBR to IR           */
        opcode = IR;        /* Store the op-code bits   */

        MAR = PC;           /* PC to MAR                */
        PC = PC + 1;        /* Increment PC             */
        MBR = memory[MAR];  /* Get the operand          */
        IR = MBR;           /* Copy MBR to IR           */
        operand = IR;       /* Store the operand bits   */
        amode = opcode & 0x03;              /* Extract the address mode bits                 */
        direction = (opcode & 0x04) >> 2;   /* Get data direction 0 = register to memory     *
                                                                  1 = memory to register     */
        opcode = opcode >> 4;                       /* Extract the 4-bit instruction code            */

        if(opcode==EXGMEM)
        {
            MAR = PC;           /* PC to MAR                */
            PC = PC + 1;        /* Increment PC             */
            MBR = memory[MAR];  /* Get the operand          */
            IR = MBR;           /* Copy MBR to IR           */
            operand2 = IR;
        }


        switch(amode)
        {
        case 0:
            source = memory[operand];
            if(opcode==EXGMEM)  source2 = memory[operand2];
            break;      /* Absolute */
        case 1:
            source = operand;
            break;      /* Literal */
        case 2:
            source = memory[A0 + operand];
            break;      /* Indexed */
        case 3:
            source = memory[PC + operand];
            break;      /* PC relative */
        }


        switch(opcode)
        {
        case MOVE:
            if(direction == 0) destination = D0;
            else D0 = source;
            if(D0 == 0) CCR = 1;
            else CCR = 0;   /* Update CCR */
            break;
        case ADD:
            if(direction == 0)
            {
                destination = D0 + source;
                if(destination == 0) CCR = 1;
                else CCR = 0;
            }
            else
            {
                D0 = D0 + source;
                if(D0 == 0) CCR = 1;
                else CCR = 0;
            }
            break;
        case SUB:
            if(direction == 0)
            {
                destination = D0 - source;
                if(destination == 0) CCR = 1;
                else CCR = 0;
            }
            else
            {
                D0 = D0 - source;
                if(destination == 0) CCR = 1;
                else CCR = 0;
            }
            break;
        case BRA:
            if(amode == 0) PC = operand;
            if(amode == 1) PC = PC + operand;
            break;
        case CMP:
            MBR = D0 - source;
            if(MBR == 0) CCR = 1;
            else CCR = 0;
            break;
        case BEQ:
            if(CCR == 1)
            {
                if(amode == 0) PC = operand;
                else if(amode == 1) PC = PC + operand;
            }
            break;
        case BNE:
            if(CCR != 1)
            {
                if(amode == 0) PC = operand;
                else if(amode == 1) PC = PC + operand;
            }
            break;
        case EXG:
            MBR = D0;
            D0 = A0;
            A0 = MBR;
            break;
        case ADDSQUARE:
            MBR=(D0*D0);
            A0=(A0*A0);
            A0=MBR+A0;

            break;
        case SWAP :


            MBR = D0>>8;
            D0=(D0<<8)+MBR;

            break;
        case EXGMEM:


            destination=source2;
            destination2=source;
            break;
        case STOP:
            run = 0;
            break;
        }
        /* Save result in memory if register to memory  */
        if(direction == 0)
        {
            switch(amode)
            {
            case 0:
                memory[operand] = destination;
                if(opcode==EXGMEM) memory[operand2]=destination2;
                break;  /* Absolute */
            case 1:
                break;                                          /* Literal */
            case 2:
                memory[A0 + operand] = destination;

                break;  /* Indexed */
            case 3:
                memory[PC + operand] = destination;
                break;  /* PC Relative */
            }
        }

    }

    printf("D0:%d\n",D0);
    printf("A0:%d\n",A0);
    printf("%d\n%d\n",memory[32],memory[54]);




}
