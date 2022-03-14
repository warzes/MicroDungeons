#pragma once

class PlayerAnimation
{
public:
	void Init();
	void Close();

	void Draw2D();
	void Update(Camera cam);

	void StartAttacking();
	bool IsAttack() const;
};