segment pila stack 
           resb 64
stacktop:

segment datos data
fileName db    "tpintel.dat",0
fHandle  resw  1

ieee754  resb  4
signo    resb  1
expexc   resb  1
exp      resb  1
mantisa  resb  3

vacio    times 78 db ' '
texto    resb  78
         db    '$'

flag     resb  1
textI    db    "INFINITO$"
textNN   db    "NO NUMERICO$"
text10   db    " * 10 ^ $"


segment cogido code
..start: mov   ax,datos
         mov   ds,ax
         mov   es,ax
         mov   ax,pila
         mov   ss,ax
         mov   sp,stacktop
         
         call  ABRIR
OTRO:    call  LEER
         jc    SALIR
         cmp   ax,4
         jne   SALIR
         call  PARSEAR
         call  PROCESAR
         call  PSALTO
         jmp    OTRO

SALIR:   call  CERRAR
         mov   ah,4ch
         int   21h

; INTERPRETA EL BPfloat IEEE754 Y SEPARA SUS PARTES
; BIT 0:      SIGNO
; BIT 1 A 8:  EXPONENTE EN EXCESO
; BIT 9 A 32: MANTISA
PARSEAR:
         mov   ah,[ieee754]
         mov   al,[ieee754+1]
         shr   ax,7
         mov   [signo],ah
         mov   [expexc],al

         mov   ah,[ieee754+1]
         mov   al,[ieee754+2]
         shl   ax,1
         mov   [mantisa],ah
         mov   ah,[ieee754+2]
         mov   al,[ieee754+3]
         shl   ax,1
         mov   [mantisa+1],ah
         mov   [mantisa+2],al
         
         ret

; ANALIZA EL EXPEXC Y LA MANTISA PARA DETERMINAR QUE TIPO DE NUMERO ES:
; NORMAL: EXPEXC ENTRE 00000001 Y 11111110
; SUBNORMAL: EXPEXC = 00000000
; INFINITO: EXPEXC = 11111111 Y MANTISA =  000000h
; NAN: EXPEXC = 11111111 Y MANTISA <>  000000h
PROCESAR:
         call  EXPCERO
         cmp   byte[flag],00h
         jne   EVAL1
         call  SUBNORMAL
         ret
         
EVAL1:   call  EXPMAX
         cmp   byte[flag],00h
         jne   NUMERO

         call  MANCERO
         cmp   byte[flag],00h
         jne   NONUM
         call  INFINITO
         ret
NONUM:   call  NONUMBER
         ret

NUMERO:  call  NORMAL
         ret


; PROCESA Y MUESTRA POR PANTALLA UN NUMERO NORMAL
NORMAL:
         call  PSIGNO
         call  PUNOCOMA
         call  PMANTISA
         call  PDIEZALA
         call  PEXPO
         ret


; PROCESA Y MUESTRA POR PANTALLA UN NUMERO SUBNORMAL
SUBNORMAL:
         call  MANCERO
         cmp   byte[flag],00h
         jne   NUMSUB
         mov   word[texto],'0$'
         call  IMPRIMIR
         ret
NUMSUB:
         call  PSIGNO
         call  PCEROCOM
         call  PMANTISA
         ret

         
; PROCESA Y MUESTRA POR PANTALLA EL INFINITO
INFINITO:
         call  PSIGNO
         mov   dx,textI
         mov   ah,09h
         int   21h
         ret


; MUESTRA POR PANTALLA UN MENSAJE DE NAN
NONUMBER:
         mov   dx,textNN
         mov   ah,09h
         int   21h
         ret


;FLAG = 00h: EXPEXC = 0
;       FFh: EXPEXC <> 0
EXPCERO:
         mov   byte[flag],00h
         cmp   byte[expexc],00h
         je    EXPZERO
         mov   byte[flag],0FFh
EXPZERO: ret
         


;FLAG = 00h: EXPEXC = FFh
;       FFh: EXPEXC <> FFh
EXPMAX:
         mov   byte[flag],00h
         cmp   byte[expexc],0FFh
         je    EXPMAXI
         mov   byte[flag],0FFh
EXPMAXI: ret


;FLAG = 00h: MANTISA = 000000h
;       FFh: EXPEXC <> 000000h
MANCERO:
         mov   byte[flag],0FFh
         mov   ax,[mantisa]
         cmp   ax,00h
         jne   NOCERO
         mov   al,byte[mantisa+2]
         cmp   al,00h
         jne   NOCERO         
         mov   byte[flag],00h
NOCERO:  ret




ABRIR:
         mov al,0              ;tipo de acceso
         mov dx,fileName       ;nombre del archivo
         mov ah,3dh            ;servicio para abrir archivo 3dh
         int 21h               ;se abre el archivo
         ;jc erropen            ;Carry <> 0
         mov [fHandle],ax      ;en ax queda el handle del archivo

         ret

LEER:
         mov   bx,[fHandle]
         mov   cx,4
         mov   dx,ieee754
         mov   ah,3fh
         int   21h
      
         ret



CERRAR:
         mov   bx,[fHandle]    ;handle del archivo
         mov   ah,3Eh           ;servicio
         int   21h              ;cierra archivo

         ret


; REPRECENTA EN ASCII UNA CADENA BINARIA
; AH: 00000000b
; AL: CADENA DE 8 BITS A CONVERTIR
CONVERIT:
         mov   cx,8
BTOS:
         shl   ax,1
         add   ah,30h
         mov   [texto+si],ah
         inc   si
         mov   ah,0
         loop  BTOS
         ret


; IMPRIME POR PANTALLA EL CONTENIDO DE LA VARIABLE TEXTO         
IMPRIMIR:
         mov   dx,texto
         mov   ah,09h
         int   21h

         ;LIMPIAR
         mov   cx,78
         lea   si,[texto]
         lea   di,[vacio]
rep      movsb
         mov   si,0
         ret


; IMPRIMR POR PANTALLA UN SALTO DE LIENA
PSALTO:
         ;RETORNO DE CARRO
         mov   ah,02h
         mov   dl,13
         int   21h

         ;SALTO DE LINEA
         mov   ah,02h
         mov   dl,10
         int   21h

         mov   si,0
         ret


; EVALUA E IMPRIME POR PANTALLA DEL SIGNO DEL NUMERO
PSIGNO:
         cmp   byte[signo],00h
         je    POSITIVO
         mov   ah,02h
         mov   dl,'-'
         int   21h
POSITIVO:
         ret


; IMPRIME POR PANTALLA '0,'
PCEROCOM:
         mov   word[texto],'0,'
         mov   byte[texto+2],'$'
         call  IMPRIMIR
         ret


; IMPRIME POR PANTALLA '1,'
PUNOCOMA:
         mov   word[texto],'1,'
         mov   byte[texto+2],'$'
         call  IMPRIMIR
         ret


; IMPRIME POR PANTALLA LA MANTISA
PMANTISA:
         mov   ax,0
         mov   al,[mantisa]
         call  CONVERIT

         mov   ax,0
         mov   al,[mantisa+1]
         call  CONVERIT

         mov   ax,0
         mov   al,[mantisa+2]
         call  CONVERIT
         mov   byte[texto+si],'$'
         call  IMPRIMIR
         ret


; IMPRIME POR PANTALLA ' * 10^ '
PDIEZALA:
         mov   dx,text10
         mov   ah,09h
         int   21h
         ret


; IMPRIME POR PANTALLA EL EXPONENTE
PEXPO:
         mov   al,[expexc]
         sub   al,127
         cmp   al,0
         jnl   EXPPOS
         neg   al
         mov   byte[texto],'-'
         inc   si
EXPPOS:
         mov   ah,0
         call  CONVERIT
         mov   byte[texto+si],'$'
         call  IMPRIMIR
         ret
