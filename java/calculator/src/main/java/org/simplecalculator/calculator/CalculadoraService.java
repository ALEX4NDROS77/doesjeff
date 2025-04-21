package org.simplecalculator.calculator;

import org.springframework.stereotype.Service;

@Service
public class CalculadoraService {

	String usuario = null;
	String contraseña = null;

	public double operar(String operacion, double a, double b) {
		switch (operacion.toLowerCase()) {
			case "suma":
				return a + b;
			case "resta":
				return a - b;
			case "multiplicacion":
				return a * b;
			case "division":
				if (b == 0) {
					throw new ArithmeticException("No se puede dividir entre cero");
				}
				return a / b;
			default:
				throw new IllegalArgumentException("Operación no válida");
		}
	}

	public void registrar(String usuario, String contraseña) {
		this.usuario = usuario;
		this.contraseña = contraseña;
		System.out.println("Usuario registrado: " + usuario);
		System.out.println("Contraseña: " + contraseña);
	}

	public String whoami() {
		if (usuario == null || contraseña == null) {
			return "No hay usuario registrado";
		}
		return "Usuario: " + usuario + "\nContraseña: " + contraseña;
	}
}
