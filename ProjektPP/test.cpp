void checkPossiblePawnTakes(s_game *gameData) {
	for (int i = 0; i < 24; i++) {
		if (gameData->pawns[i][gameData->turn - 1] > 0) {
			establishPossibleDest()
		}
		else
			break;
	}
}