package org.simplecalculator.calculator;

import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestParam;

@RestController
@RequestMapping("/api/calculadora")
public class CalculadoraController {
	private final CalculadoraService service;

	public CalculadoraController(CalculadoraService service) {
		this.service = service;
	}

	@GetMapping("/{operacion}")
	public double calcular(
			@PathVariable("operacion") String operacion,
			@RequestParam("num1") double num1,
			@RequestParam("num2") double num2) {
		return service.operar(operacion, num1, num2);
	}

	@PostMapping("/registro")
	public void registrar(
			@RequestParam("user") String usuario,
			@RequestParam("pass") String contraseña) {
		service.registrar(usuario, contraseña);
	}

	@GetMapping("/usuario")
	public String whoami() {
		return service.whoami();
	}
}
