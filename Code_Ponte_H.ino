/* 
Cada pulsación do pulsador permite percorrer sucesivamente 
os estados do motor: 
(a) xira en sentido dereito (antihorario), 
(b) frea inmediatamente, 
(c) xira en sentido inverso (horario), 
(d) frea inmediatamente. 
Ten que haber un estado máis, de inicio ou estado -1, 
que corresponda con 'freado baixo a propia inercia'. 
O automatismo migra a este último estado sempre que 
pasan máis de 15 s desde que se fixo un paro inmediato. 
Por suposto, desde este estado sempre migrará ao indicado en (a).

*/

#define BOTON 3
#define S1 13
#define S2 12
#define S3 11
#define S4 10

bool pulsa = false;

int estado = -1;      

int contador = 0;	
 

void setup()
    
{
  pinMode(BOTON, INPUT); 
  pinMode(S1, OUTPUT);         
  pinMode(S2, OUTPUT);           
  pinMode(S3, OUTPUT);  
  pinMode(S4, OUTPUT); 
  Serial.begin(9600);			
}
    
    
  void XiraDereita() {
    
    digitalWrite(S1, HIGH);      // Xira Dereita.
    digitalWrite(S4, HIGH);
    digitalWrite(S3,LOW);        // Enclavamos os outros dous.
    digitalWrite(S2,LOW);
}
   void XiraEsquera() {
    
    digitalWrite(S3, HIGH);      // Xira Dereita.
    digitalWrite(S2, HIGH);
    digitalWrite(S1,LOW);        // Enclavamos os outros dous.
    digitalWrite(S4,LOW);
}
   void ParoInmediato() {
     
    digitalWrite(S1, HIGH);
    digitalWrite(S2, HIGH);
    digitalWrite(S3, LOW);
    digitalWrite(S4, LOW);
    contador = 0;               // Reinicia o contador
}

   void ParoInercia() {         // Todo apagado
  
    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    digitalWrite(S4, LOW);
}


void loop(){
  
/*************** Comprobamos o PULSADOR. ****************/
  
  pulsa = digitalRead(BOTON);    // Miro se o botón está pulsado.
 
  if ( pulsa == true ){          // Se o botón se pulsa...
    
    
 // Se estamos en inercia, imos ao estado 0 directamente
    
    if (estado == -1) {
      
      estado = 0;
      
    } else { 
    
    estado = (estado +1) % 4;    // Ciclo 0 -> 1 -> 2 -> 3
    
    }
    
   while(digitalRead(BOTON)){ 
      delay(20);                 /* Aseguro unha única lectura.
      							    Por eso lle poño unha espera de 20ms 
							        entre lecturas de pulsación.*/
    }

  
    // Cambia o estado de acordo co valor de 'estado'   
    switch (estado){
      
    case 0:
    	XiraDereita ();          // Xira horario.
      	break;
    case 1:
        ParoInmediato();		 // Estado Paro Intempestivo.
      	break;
    case 2:
    	XiraEsquera ();          // Xira Antihorario.
    	break;
    case 3:
      	ParoInmediato();		 // Estado Paro Intempestivo.
   	 	break;  
  }
  }
  
/*********** Condición do Paro Intempestivo ************/
  
  if (estado == 0 || estado == 2) {
   
  contador ++;
    
  }
  
    if (contador == 150){
      
    ParoInercia();          // Freado por inercia
      
    contador = 0;           // Reiniciamos Contador  
   
    estado = -1;            // Volvemos ó estado de Inicio
  
  }
  
/********************  Monitor Serie  ********************/
Serial.print("Estado: ");
  Serial.print(estado);
  Serial.print(" | Tempo de paro: ");
  Serial.println(contador);
}
