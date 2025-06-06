with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Numerics.Discrete_Random;

procedure ej3 is

   -- Variables globales
   Cola : Integer := 0;           -- Cantidad de personas en cola
   CajasOcupadas : Integer := 0;  -- Cantidad de cajas ocupadas
   NumPersona : Integer := 1;     -- ID incremental para las personas
   
   -- Generador de números aleatorios para tiempo en caja
   subtype Tiempo_Range is Integer range 1 .. 5;
   package Random_Tiempo is new Ada.Numerics.Discrete_Random(Tiempo_Range);
   Gen : Random_Tiempo.Generator;
   
   
   -- Función para agregar persona a la cola
   procedure ColaDisponible is
   begin
      if Cola = 10 then
         Put_Line("Cola de espera llena, no se permiten más personas por ahora.");
      else
         Cola := Cola + 1;
         Put_Line("Usuario" & Integer'Image(NumPersona) & " entra a la cola de espera.");
         NumPersona := NumPersona + 1;
      end if;
   end ColaDisponible;
   
   -- Función para mover persona de cola a caja
   procedure CajaDisponible is
      PersonaAtendida : Integer;
   begin
      if (CajasOcupadas < 4) and (Cola > 0) then
         CajasOcupadas := CajasOcupadas + 1;
         Cola := Cola - 1;
         PersonaAtendida := NumPersona - Cola - CajasOcupadas;
         Put_Line("Usuario" & Integer'Image(PersonaAtendida) & " sale de la cola de espera.");
         Put_Line("Persona" & Integer'Image(PersonaAtendida) & " ingresó a la caja" & Integer'Image(CajasOcupadas) & ".");
      end if;
   end CajaDisponible;
   
   -- Función que simula el tiempo en caja
   procedure EnCaja(PersonaID : Integer; CajaNum : Integer) is
      TiempoAleatorio : Integer;
   begin
      TiempoAleatorio := Random_Tiempo.Random(Gen);
      Put_Line("Persona" & Integer'Image(PersonaID) & " está siendo atendida en caja" & Integer'Image(CajaNum) & " por" & Integer'Image(TiempoAleatorio) & " segundos.");
      -- Aquí simularíamos el delay, pero por simplicidad solo mostramos el mensaje
   end EnCaja;
   
   -- Función para liberar caja
   procedure FueraDeCaja(PersonaID : Integer; CajaNum : Integer) is
   begin
      CajasOcupadas := CajasOcupadas - 1;
      Put_Line("Persona" & Integer'Image(PersonaID) & " saliendo de la caja" & Integer'Image(CajaNum) & ".");
   end FueraDeCaja;
   
   -- Variables para el ejemplo
   TotalPersonas : Integer;
   
begin
   -- Inicializar generador aleatorio
   Random_Tiempo.Reset(Gen);
   
   Put("Ingrese la cantidad de personas a crear: ");
   Get(TotalPersonas);
   New_Line;
   
   -- Ejemplo simple de uso de las funciones
   Put_Line("=== Simulación Simple ===");
   
   -- Llenar la cola primero
   for I in 1..10 loop
      ColaDisponible;
   end loop;
   
   New_Line;
   Put_Line("=== Intentando agregar más personas ===");
   ColaDisponible; -- Debería mostrar mensaje de cola llena
   
   New_Line;
   Put_Line("=== Moviendo personas a cajas ===");
   
   -- Mover algunas personas a las cajas
   for I in 1..4 loop
      CajaDisponible;
   end loop;
   
   New_Line;
   Put_Line("=== Estado actual ===");
   Put_Line("Personas en cola:" & Integer'Image(Cola));
   Put_Line("Cajas ocupadas:" & Integer'Image(CajasOcupadas));
   
   New_Line;
   Put_Line("=== Simulando tiempo en cajas ===");
   EnCaja(1, 1);
   EnCaja(2, 2);
   EnCaja(3, 3);
   EnCaja(4, 4);
   
   New_Line;
   Put_Line("=== Liberando cajas ===");
   FueraDeCaja(1, 1);
   FueraDeCaja(2, 2);
   FueraDeCaja(3, 3);
   FueraDeCaja(4, 4);
   
   New_Line;
   Put_Line("Estado final:");
   Put_Line("Personas en cola:" & Integer'Image(Cola));
   Put_Line("Cajas ocupadas:" & Integer'Image(CajasOcupadas));
   
end ej3;