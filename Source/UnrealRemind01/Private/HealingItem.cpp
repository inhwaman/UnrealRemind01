// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "PlayerCharacter.h"
AHealingItem::AHealingItem()
{
	HealingAmount = 20;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealingAmount);
		}
		

		DestroyItem();
	}
}
