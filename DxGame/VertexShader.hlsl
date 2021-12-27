

float4 vsmain(float4 pos : POSITION) : SV_POSITION
{
	if (pos.y > 0.0f && pos.y < 1.0f)
		pos.x += 0.25f;
	if (pos.x > 0.0f && pos.y < 0.0f)
		pos.y += 0.25f;
	if (pos.x < 0.0f && pos.y > 0.0f)
		pos.y -= 0.25f;
	return pos;
}