
SRC := main.c utils.c renderer.c jogador.c combate.c grupoinimigos.c baralho.c teclado.c


TARGET := game.out

game: $(SRC)
	gcc $(SRC) -o $(TARGET) \
		-lm -g \
		`pkg-config allegro-5 allegro_main-5 \
        allegro_font-5 allegro_ttf-5 \
        allegro_image-5 allegro_primitives-5 \
        --libs --cflags`


game-win: $(SRC)
	gcc $(SRC) -o $(TARGET) \
		-lm -g \
		$(shell pkg-config allegro-5 allegro_main-5 \
		allegro_font-5 \
		allegro_image-5 allegro_primitives-5 \
		--libs --cflags)