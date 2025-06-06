with Ada.Text_IO;
use Ada.Text_IO;
with Ada.Integer_Text_IO;
use Ada.Integer_Text_IO;
with Ada.Numerics.Discrete_Random;

procedure Main is

   totalPersonas : Integer; -- n ingresado
   cola : Integer := 0; -- contador de personas en la cola
   proxPersonaID : Integer := 1; -- id para la prox persona que salga de la cola
   personasAtendidas : Integer := 0; -- personas ya atendidas
   personasFueraDeCola : Integer; -- personas que aún no entraron a la cola
   colaLlena : Boolean := false; -- flag para indicar si la cola está llena
   puedenProcesarCajas : Boolean := false; -- flag para que las cajas empiecen pq sino empiezan antes de que se cargue la cola

   -- generador números del 1 al 5 para el tiempo en caja
   type tiempo5 is range 1 .. 5;
   package random5 is new Ada.Numerics.Discrete_Random(tiempo5);
   gen5 : random5.Generator;

   -- generador números del 1 al 3 para reintentar entrar a la cola cuando está llena
   type tiempo3 is range 1 .. 3;
   package random3 is new Ada.Numerics.Discrete_Random(tiempo3);
   gen3 : random3.Generator;

   -- semáforo
   task type Semaforo is
      entry wait;
      entry signal;
      entry init(n : Integer);
   end Semaforo;

   task body Semaforo is
      count : Integer;
   begin
      accept init(n : Integer) do
         count := n;
      end init;

      loop
         select
            accept wait do
               count := count - 1;
            end wait;
         or
            accept signal do
               count := count + 1;
            end signal;
         or
            terminate;
         end select;
      end loop;
   end Semaforo;

   semCola : Semaforo;    -- semáforo para controlar la cola
   semCaja : Semaforo;    -- semáforo para controlar las cajas
   semMutex : Semaforo;

   -- task para agregar personas
   task type GeneradorPersonas;
   task body GeneradorPersonas is
      t3 : tiempo3;
      IDactual : Integer := 1;
   begin
      while cola < 10 and personasFueraDeCola > 0 loop
         delay 1.0;  -- crear una persona por segundo

         semMutex.wait;
         Put_Line("Persona" & Integer'Image(IDactual) & " entra a la cola de espera.");
         cola := cola + 1;
         personasFueraDeCola := personasFueraDeCola - 1;
         IDactual := IDactual + 1;

         if cola = 10 and personasFueraDeCola > 0 then
            colaLlena := true;
            Put_Line("Cola de espera llena, no se permiten más personas por ahora.");
         end if;

         if personasFueraDeCola = 0 or cola = 10 then
            puedenProcesarCajas := true;
         end if;

         semMutex.signal;
      end loop;

      while personasFueraDeCola > 0 loop
         -- intentar entrar a la cola
         loop
            select
               semCola.wait;
               exit;
            else
               t3 := random3.Random(gen3);
               delay Duration(t3);
            end select;
         end loop;

         delay 1.0;  -- crear una persona por segundo

         semMutex.wait;
         Put_Line("Persona" & Integer'Image(IDactual) & " entra a la cola de espera.");
         cola := cola + 1;
         personasFueraDeCola := personasFueraDeCola - 1;
         IDactual := IDactual + 1;
         semMutex.signal;
      end loop;
   end GeneradorPersonas;

   -- Task para las cajas
   task type Caja(ID : Positive);
   task body Caja is
      personaID : Integer;
      tiempo : tiempo5;
   begin
      while not puedenProcesarCajas loop
         delay 0.1;
      end loop;

      loop
         exit when personasAtendidas >= totalPersonas;

         semMutex.wait;
         if cola = 0 then
            semMutex.signal;
            delay 0.5;
         else
            cola := cola - 1;
            personaID := proxPersonaID;
            proxPersonaID := proxPersonaID + 1;
            semMutex.signal;
            semCola.signal;

            Put_Line("Persona" & Integer'Image(personaID) & " sale de la cola de espera.");

            semCaja.wait;
            Put_Line("Persona" & Integer'Image(personaID) & " ingresó a la caja" & Integer'Image(ID));

            tiempo := random5.Random(gen5);
            delay Duration(tiempo);

            Put_Line("Persona" & Integer'Image(personaID) & " saliendo de la caja" & Integer'Image(ID));
            semCaja.signal;

            semMutex.wait;
            personasAtendidas := personasAtendidas + 1;
            semMutex.signal;
         end if;
      end loop;
   end Caja;

begin
   -- inicializo todo

   random5.Reset(gen5);
   random3.Reset(gen3);

   Put("Ingrese la cantidad de personas a crear: ");
   Get(totalPersonas);
   Skip_Line;

   personasFueraDeCola := totalPersonas;

   semCola.init(10);    -- 10 espacios en cola
   semCaja.init(4);     -- 4 cajas disponibles
   semMutex.init(1);    -- mutex binario

   declare
      generador : GeneradorPersonas;
      caja1 : Caja(1);
      caja2 : Caja(2);
      caja3 : Caja(3);
      caja4 : Caja(4);
   begin
      null;
   end;

   while personasAtendidas < totalPersonas loop
      delay 1.0;
   end loop;

   Put_Line("Todo salió bien!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
end Main;
