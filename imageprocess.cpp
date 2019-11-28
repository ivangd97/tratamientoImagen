/***************************************************************************
 *   Copyright (C) 2011 by pilar   *
 *   pilarb@unex.es   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "imageprocess.h"


void imageprocess::copiar(uchar * imgO, uchar * imgD)
{
	asm volatile(
		"movl %0, %%esi \n\t"	
		"movl %1, %%edi \n\t"	
        "movl $320*240, %%ecx\n\t"

		"bcopia:"
            "movb (%%esi), %%al \n\t"              /* movemos contenido de esi a al */
            "movb %%al, (%%edi) \n\t"              /* movemos al a el contenido de edi */
            "incl %%esi \n\t"
            "incl %%edi \n\t"

		"loop bcopia \n\t"
		: 
		: "m" (imgO),	"m" (imgD)
		: "%eax", "%ecx", "%esi", "%edi", "memory"

);


}


void imageprocess::negar(uchar * imgO, uchar * imgD)
{
	asm volatile(
        "movl %0, %%esi \n\t"
        "movl %1, %%edi;"
        "movl $320*240, %%ecx\n\t"

        "bucleNegar:"
            "mov $255, %%al;"                       /* movemos 255 a al para que el pizel nos quede negado */
            "sub (%%esi), %%al \n\t"                /* le restamos al pixel al */
            "movb %%al, (%%edi) \n\t"               /* movemos el pixel negado a la imagen destino */

            "incl %%esi \n\t"
            "incl %%edi \n\t"
        "loop bucleNegar;"

        :
        : "m" (imgO),	"m" (imgD)
        : "%eax", "%esi","%edi","%ecx","memory"

);


}

void imageprocess::recortar(uchar * imgO, uchar * imgD, int cI, int fI, int w, int h)
{
    int despFila;
    int despColum;
    int dos;

    asm volatile(
        "movl %0, %%esi ;"
        "movl %1, %%edi;"
        "movl %3, %%eax;"                   /* eax registro auxiliar para cuentas */
        "movl $2, %8;"                      /* muevo un 2 a la variable local para simplificar calculos posteriores */
        "imull $320, %%eax;"
        "addl %2, %%eax;"
        "addl %%eax, %%esi;"                /* cuentas necesarias para dirOrigen */

        "movl $240, %%eax;"
        "subl %5, %%eax;"
        "movl $0, %%edx;"                   /* edx por posible contenido error */
        "divl %8;"
        "movl %%eax, %6;"                   /* cuentas necesarias para despFila */

        "movl $320, %%eax;"
        "subl %4, %%eax;"
        "movl $0, %%edx;"
        "divl %8;"
        "movl %%eax, %7;"                  /* cuentas necesarias para despColumna */

        "movl $0, %%ecx;"                 /* i para el primer for */
        "bucleRecortar1:"

            "cmpl %6, %%ecx;"               /*principio condicion primer if */
            "jl elseRec1;"                  /* si ecx menor que despfila salta */
            "movl $240, %%eax;"
            "subl %%ecx, %%eax;"
            "cmpl %6, %%eax;"               /* si despfila menor o igual que 240 menos ecx salta */
            "jle elseRec1;"                 /* final condicion primer if */

                "movl $0, %%edx;"           /* indice para segundo bucle */
                "bucleRecortar2:"

                "cmpl %7, %%edx;"           /* principio condicion segundo if */
                "jl elseRec2;"              /* si edx menor que despColum salta */
                "movl $320, %%eax;"
                "subl %%edx, %%eax;"
                "cmpl %7, %%eax;"           /* si 320 menos edx menor o igual que despColum salta */
                "jle elseRec2;"                     /* final condicion segundo if */

                    "movl (%%esi), %%eax;"          /* (dirO = dirD) */
                    "movl %%eax, (%%edi);"
                    "incl %%esi;"                   /* dirO++ */
                    "jmp incrementar;"

                "elseRec2:"
                "movl $0, (%%edi);"                 /* pone el pixel en negro */

                "incrementar:"
                "incl %%edi;"                       /* incrementa la direccion destino siempre */

                "incl %%edx;"                       /* incrementa el indice del bucle */
                "cmpl $320, %%edx;"
                "jl bucleRecortar2;"

                "finBucleRec2:"
                    "movl %7, %%eax;"
                    "imull $2, %%eax;"
                    "addl %%eax, %%esi;"            /* dirO = dirO +despCol*2  */
                    "jmp finBucleRec1;"

            "elseRec1:"
                    "movl $0, %%edx;"               /* indice bucle de else */
                    "bucleRecortar3:"

                        "movl $0, (%%edi);"         /* contenido de dirD = 0 */
                        "incl %%edi;"               /* incrementar dirDest */
                        "incl %%edx;"
                        "cmpl $320, %%edx;"
                        "jl bucleRecortar3;"

            "finBucleRec1:"
            "incl %%ecx;"
            "cmpl $240, %%ecx;"
            "jl bucleRecortar1;"                    /* incrementa el indice del primer bucle */

        :
        : "m" (imgO),	"m" (imgD), "m" (cI), "m" (fI), "m" (w), "m" (h), "m" (despFila), "m" (despColum), "m" (dos)
        : "%eax","%edx","%esi","%ecx","%edx", "%edi", "memory"

);


}


void imageprocess::voltearHorizontal(uchar * imgO, uchar * imgD)
{
	asm volatile(
         "movl %0, %%esi;"
         "movl %1, %%edi;"
         "movl $240, %%ecx;"
         "addl $319, %%esi;"                        /*movemos el pixel de dirO al final de la primera fila */

         "bucleVoltearH1:"

                "movl $0, %%edx;"
                "bucleVoltearH2:"

                      "movb (%%esi), %%al;"
                      "movb %%al, (%%edi);"
                      "decl %%esi ;"                 /* decrementamos el origen para ir de derecha a izquierda */
                      "incl %%edi ;"                 /* incrementamos el destino para ir de izquierda a derecha */
                      "incl %%edx;"                     /* incrementamos el indice */
                      "cmpl $320, %%edx;"
                      "jne bucleVoltearH2;"

                "addl $640, %%esi;"

         "loop bucleVoltearH1;"

        :
        : "m" (imgO),	"m" (imgD)
        : "%eax", "%esi","%edi","%ecx","%edx","memory"
);


}

void imageprocess::voltearVertical(uchar * imgO, uchar * imgD)
{
	asm volatile(
        "movl %0, %%esi;"
        "movl %1, %%edi;"
        "movl $239*320, %%eax;"                         /* movemos el pixel de origen a la esquina inferior izquierda */
        "addl %%eax, %%esi;"

        "movl $240, %%ecx;"                             /* indice 1 */
        "bucleVoltearV1:"

              "movl $0, %%edx;"
              "bucleVoltearV2:"

                    "movb (%%esi), %%al;"
                    "movb %%al, (%%edi);"
                    "incl %%esi;"                   /* incrementamos origen */
                    "incl %%edi;"                   /* incrementamos destino */
                    "incl %%edx;"
                    "cmpl $320, %%edx;"
                    "jne bucleVoltearV2;"

              "subl $640, %%esi;"

         "loop bucleVoltearV1;"

         :
         : "m" (imgO),	"m" (imgD)
         : "%eax", "%esi","%edi","%ecx","%edx", "%ebx", "memory"
);


}


void imageprocess::girarIzquierda(uchar * imgO, uchar * imgD)
{

    asm volatile(
         "movl %0, %%esi ;"
         "movl %1, %%edi;"
         "movl $-120, %%eax;"   /* eax para bucle 1 */

         "bucleGirarIzq1:"

                "movl $-40*320, %%ecx;"
                "addl $160, %%ecx;"     /* guardo posPixel en ecx */
                "subl %%eax, %%ecx;"
                "movl $-160, %%edx;"    /* edx para bucle 2 */

                "bucleGirarIzq2:"

                    "cmpl $0, %%ecx;"           /*comienzo de la condicion */
                    "jl else;"
                    "cmpl $320*240, %%ecx;"
                    "jge else;"                  /* final de la condicion */
                    "movl %%esi, %%ebx;"
                    "addl %%ecx, %%ebx;"
                    "movl (%%ebx), %%ebx;"
                    "movl %%ebx,(%%edi);"
                    "jmp finBucle;"

                    "else:"
                        "movl $0, (%%edi);"

                    "finBucle:"
                    "incl %%edi;"
                    "addl $320, %%ecx;"

                "incl %%edx;"                    /* j++ */
                "cmpl $160, %%edx;"
                "jl bucleGirarIzq2;"

        "incl %%eax;"                           /* i++ */
        "cmpl $120, %%eax;"
        "jl bucleGirarIzq1;"

        :
        : "m" (imgO),	"m" (imgD)
        : "%eax", "%esi","%edi","%ecx","%edx", "%ebx", "memory"

);


}

void imageprocess::girarDerecha(uchar * imgO, uchar * imgD)
{

	asm volatile(
           "movl %0, %%esi;"
           "movl %1, %%edi;"
           "movl $-120, %%eax;"   /* eax para bucle 1 */

            "bucleGirarDer1:"

                "movl $280*320, %%ecx;"
                "addl $160, %%ecx;"     /* guardo posPixel en ecx */
                "addl %%eax, %%ecx;"
                "movl $-160, %%edx;"    /* edx para bucle 2 */

                "bucleGirarDer2:"

                    "cmpl $0, %%ecx;"           /*comienzo de la condicion */
                    "jl elseDer;"
                    "cmpl $320*240, %%ecx;"
                    "jge elseDer;"                  /* final de la condicion */
                    "movl %%esi, %%ebx;"
                    "addl %%ecx, %%ebx;"
                    "movl (%%ebx), %%ebx;"
                    "movl %%ebx,(%%edi);"
                    "jmp finBucleDer;"

                "elseDer:"
                    "movl $0, (%%edi);"

                "finBucleDer:"
                "incl %%edi;"
                "subl $320, %%ecx;"

            "incl %%edx;"                    /* j++ */
            "cmpl $160, %%edx;"
            "jl bucleGirarDer2;"

        "incl %%eax;"                           /* i++ */
        "cmpl $120, %%eax;"
        "jl bucleGirarDer1;"


        :
        : "m" (imgO),	"m" (imgD)
        : "%eax", "%esi","%edi","%ecx","%edx", "%ebx", "memory"

);


}

void imageprocess::calcularHistograma(uchar * imgO, int * histoO)
{
    asm volatile(
        "\n\t"

        :
        : "m" (imgO),	"m" (histoO)
        : "memory"

);

}

void imageprocess::calcularUmbral(int * histoO, uchar & umbral)
{

    asm volatile(
        "\n\t"

        : "=m" (umbral)
        : "m" (histoO)
        : "memory"
    );

}

void imageprocess::umbralizar(uchar * imgO, uchar umbral, uchar * imgD)
{
    asm volatile(
        "\n\t"

        :
        : "m" (imgO), "m" (umbral), "m" (imgD)
        : "memory"
    );

}

