	.arch msp430g2553

	.p2align 1,0

	.text
	.global jump
	.extern buzzer_set_period
	.extern birdX
	.extern birdY
	.extern jumpForce
	.extern birdWidth
	.extern birdHeight
	.extern jumpForce
	.extern mute
    .extern fillRectangle


jump:
    sub &jumpForce, &birdY
	mov &birdX, r12
    mov &birdY, r13
    add &jumpForce, r13
    mov &birdWidth, r14
    mov &birdHeight, r15
    push #0xf800
    call #fillRectangle

	cmp #0, &mute
	jnz on

	pop r0

on:
    mov #0, r12
    call #buzzer_set_period
    mov #3817, r12
    call #buzzer_set_period
	pop r0
