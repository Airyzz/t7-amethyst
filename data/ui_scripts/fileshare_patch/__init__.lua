local function __FUNC_7F313_( f674_arg0, f674_arg1, f674_arg2 )
		local f674_local0 = f674_arg2 - 1
		local f674_local1 = f674_local0 % f674_arg1.numElements + 1
		local f674_local2 = f674_arg2 % f674_arg1.numElements
		if f674_arg1.hasCreateButton == true then
			f674_local0 = f674_local0 - 1
		end
		local f674_local3 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "isPublishNew" )
		local f674_local4 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "showPlusImage" )
		local f674_local5 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "showFileImage" )
		local f674_local6 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "plusImageSrc" )
		local f674_local7 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "fileImage" )
		local f674_local8 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "fileId" )
		local f674_local9 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "renderFileId" )
		local f674_local10 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "gameType" )
		local f674_local11 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "gameTypeImage" )
		local f674_local12 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "showGameTypeImage" )
		local f674_local13 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "slot" )
		local f674_local14 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "index" )
		local f674_local15 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "weaponName" )
		local f674_local16 = nil
		if CoD.FileshareUtility.GetIsGroupsMode( f674_arg0 ) then
			f674_local16 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "featureIsProcessing" )
		end
		local f674_local17 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "mapName" )
		local f674_local18 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "duration" )
		Engine.SetModelValue( Engine.CreateModel( f674_arg1.fileshareData[f674_local1].elementModel, "uiModelIndex" ), f674_local1 )
		Engine.SetModelValue( Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "codeIndex" ), f674_local0 )
		Engine.SetModelValue( Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "totalItems" ), f674_arg1.itemsCount )
		Engine.SetModelValue( Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "showDetailsViewSpinner" ), 0 )
		Engine.SetModelValue( f674_local8, nil )
		if f674_arg1.hasCreateButton == true and f674_arg2 == 1 then
			Engine.SetModelValue( f674_local3, true )
			Engine.SetModelValue( f674_local4, 1 )
			Engine.SetModelValue( f674_local7, "" )
			Engine.SetModelValue( f674_local5, 0 )
			Engine.SetModelValue( f674_local9, 0 )
			Engine.SetModelValue( f674_local6, "uie_t7_icon_codpoints" )
			Engine.SetModelValue( f674_local18, 0 )
			Engine.SetModelValue( f674_local17, "" )
			Engine.SetModelValue( f674_local12, 0 )
			Engine.SetModelValue( f674_local11, "" )
			Engine.SetModelValue( f674_local15, "" )
			local f674_local19 = CoD.FileshareUtility.GetInShowcaseManager( f674_arg0 )
			local f674_local20 = Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, "fileName" )
			if f674_local19 and f674_local19 == true then
				Engine.SetModelValue( f674_local20, Engine.Localize( "MENU_FILESHARE_BUY_SLOTS" ) )
			else
				Engine.SetModelValue( f674_local20, Engine.Localize( "MENU_FILESHARE_PUBLISH_NEW" ) )
			end
		else
			Engine.SetModelValue( f674_local3, false )
			Engine.SetModelValue( f674_local7, "" )
			Engine.SetModelValue( f674_local4, 0 )
			local f674_local19 = CoD.FileshareUtility.GetCurrentCategory()
			local f674_local20 = CoD.FileshareUtility.GetIsCommunityMode( f674_arg1.controller )
			local f674_local21, f674_local22 = CoD.FileshareUtility.GetCurrentFilter()
			local f674_local23 = nil
			if f674_local20 or CoD.FileshareUtility.GetIsGroupsMode( f674_arg1.controller ) or f674_local19 == "recentgames" then
				if f674_local19 == "recentgames" then
					f674_local23 = Engine.FileshareGetCommunitySlotData( f674_arg1.controller, "film", f674_local21, f674_local22, f674_local0 )
				else
					f674_local23 = Engine.FileshareGetCommunitySlotData( f674_arg1.controller, f674_local19, f674_local21, f674_local22, f674_local0 )
				end
			else
				f674_local23 = Engine.FileshareGetSlotData( f674_arg1.controller, f674_local19, f674_local21, f674_local22, f674_local0 )
			end
			CoD.FileshareUtility.SetCategoryCurrentPage( f674_arg0, f674_local19, f674_local0 )
			if CoD.FileshareUtility.GetIsGroupsMode( f674_arg0 ) then
				f674_local19 = f674_local23.fileCategory
			end
			if f674_local19 == "recentgames" or f674_local19 == "film" or f674_local19 == "customgame" then
				Engine.SetModelValue( f674_local12, 1 )
			else
				Engine.SetModelValue( f674_local12, 0 )
			end
			if f674_local19 == "clip" or f674_local19 == "clip_private" or f674_local19 == "recentgames" or f674_local19 == "film" or f674_local19 == "customgame" or f674_local19 == "screenshot" then
				Engine.SetModelValue( f674_local5, 0 )
			else
				Engine.SetModelValue( f674_local5, 1 )
			end
			local f674_local24 = nil
			local f674_local25
			if f674_local19 ~= "screenshot_private" and f674_local19 ~= "screenshot" and f674_local19 ~= "clip_private" and f674_local19 ~= "clip" then
				f674_local25 = false
			else
				f674_local25 = true
			end
			local f674_local26 = CoD.FileshareUtility.GetForceRefreshThumbnails()
			if f674_local25 == true then
				f674_local24 = Engine.GetModelValue( f674_local8 )
			end
			for f674_local30, f674_local31 in ipairs( CoD.FileshareUtility.FileProperties ) do
				if f674_local23[f674_local31] ~= nil then
					Engine.SetModelValue( Engine.GetModel( f674_arg1.fileshareData[f674_local1].elementModel, f674_local31 ), f674_local23[f674_local31] )
				end
			end
			f674_local27 = ""
			if f674_local23.weaponIndex ~= nil and f674_local23.weaponIndex ~= 0 then
				f674_local27 = Engine.ToUpper( Engine.Localize( Engine.GetItemName( f674_local23.weaponIndex ) ) )
			end
			Engine.SetModelValue( f674_local15, f674_local27 )
			CoD.FileshareUtility.SetupDefaultNameAndDescription( f674_arg1.fileshareData[f674_local1].elementModel )
			if f674_local25 == true then
				if Engine.GetModelValue( Engine.CreateModel( Engine.GetModelForController( f674_arg0 ), "groups.showFullScreenshot" ) ) then
					if f674_local23.fileId ~= CoD.currentScreenshot then
						FileshareDestroyScreenshot( f674_arg0 )
						f674_local28 = f674_local23.fileSize
						if not f674_local28 then
							f674_local28 = 0
						end
						if f674_local23.fileId ~= Engine.DefaultID64Value() then
							Engine.DownloadScreenshot( f674_arg0, f674_local23.fileId, f674_local28 )
							CoD.currentScreenshot = f674_local23.fileId
						end
					end
				elseif f674_local24 ~= f674_local23.fileId or f674_local26 == true then
					if f674_local24 ~= nil then
						Engine.ResetThumbnail( f674_local24 )
					end
					Engine.AddThumbnail( f674_arg0, Enum.ScreenshotViewType.UI_SCREENSHOT_TYPE_THUMBNAIL, f674_local23.fileId, f674_local23.fileSummarySize )
				end
			end
			if f674_local23.isValid == false then
				Engine.SetModelValue( f674_local12, 0 )
			end
			if f674_local16 then
				f674_local28 = false
				if CoD.perController[f674_arg0].selectedGroup then
					f674_local29 = f674_local23.fileId
					f674_local31 = tostring( Engine.GetModelValue( Engine.GetModel( CoD.perController[f674_arg0].selectedGroup, "groupId" ) ) )
					if CoD.groupUnfeaturedList and CoD.groupUnfeaturedList[f674_local31] then
						for f674_local35, f674_local36 in ipairs( CoD.groupUnfeaturedList[f674_local31] ) do
							if f674_local36 == f674_local29 then
								f674_local28 = true
								break
							end
						end
					end
				end
				Engine.SetModelValue( f674_local16, f674_local28 )
			end
			Engine.SetModelValue( Engine.CreateModel( f674_arg1.fileshareData[f674_local1].elementModel, "action" ), function ( f675_arg0, f675_arg1, f675_arg2, f675_arg3, f675_arg4 )
				if not CoD.FileshareUtility.IsValid( f675_arg2, f675_arg1 ) then
					LuaUtils.UI_ShowErrorMessageDialog( f675_arg2, "MENU_FILESHARE_INVALID_ITEM" )
				elseif f674_local19 == "film" or f674_local19 == "recentgames" or f674_local19 == "film_private" or f674_local19 == "clip" or f674_local19 == "clip_private" then
					if not IsInTheaterMode() then
						LuaUtils.UI_ShowErrorMessageDialog( f675_arg2, "MENU_FILESHARE_PLAY_ONLY_IN_THEATER" )
					else
						local f675_local0 = Engine.GetModelValue( Engine.GetModel( f675_arg1:getModel(), "mainMode" ) )
						if f675_local0 ~= Engine.CurrentSessionMode() then
							local f675_local1 = ""
							local registerVal8 = Engine.GetAbbreviationForMode(f675_local0)
							Engine.SwitchMode(arg2, registerVal8)
							-- if f675_local0 == Enum.eModes.MODE_ZOMBIES then
								-- f675_local1 = Engine.Localize( "MENU_FILESHARE_PLAY_ONLY_IN_SPECIFIC_MODE", "MENU_ZOMBIE" )
							-- else
								-- f675_local1 = Engine.Localize( "MENU_FILESHARE_PLAY_ONLY_IN_SPECIFIC_MODE", "MENU_MULTIPLAYER" )
							-- end
							-- LuaUtils.UI_ShowErrorMessageDialog( f675_arg2, f675_local1 )
							CoD.FileshareUtility.LoadTheaterFile( f675_arg0, f675_arg1, f675_arg2, f675_arg3, f675_arg4 )
							if f675_arg4.menuName == "Theater_SelectFilm" then
								Close( f675_arg0, f675_arg2 )
							else
								GoBackToMenu( f675_arg4, f675_arg2, "Lobby" )
								Engine.SendClientScriptNotify( f675_arg2, "menu_change" .. Engine.GetLocalClientNum( f675_arg2 ), "Main", "closeToMenu" )
							end
						else
							CoD.FileshareUtility.LoadTheaterFile( f675_arg0, f675_arg1, f675_arg2, f675_arg3, f675_arg4 )
							if f675_arg4.menuName == "Theater_SelectFilm" then
								Close( f675_arg0, f675_arg2 )
							else
								GoBackToMenu( f675_arg4, f675_arg2, "Lobby" )
								Engine.SendClientScriptNotify( f675_arg2, "menu_change" .. Engine.GetLocalClientNum( f675_arg2 ), "Main", "closeToMenu" )
							end
						end
					end
				elseif f674_local19 == "customgame" then
					if IsCustomMPLobby() then
						OpenOverlay( f675_arg0, "CustomGamesLoadFileshareItem", f675_arg2 )
					else
						LuaUtils.UI_ShowErrorMessageDialog( f675_arg2, "MENU_CUSTOMGAMES_CANNOT_VIEW_DESC" )
					end
				else
					FileshareSetSelectedItem( f675_arg0, f675_arg1, f675_arg2, "true" )
					if FileshareCanOpenDetailsView( f675_arg1, f675_arg2 ) == true then
						FileshareOpenFullscreenView( f675_arg0, f675_arg1, f675_arg2, f675_arg4 )
					end
				end
			end )
		end
		return f674_arg1.fileshareData[f674_local1].elementModel
	end


DataSources["FilesharePublishedList"].getItem = __FUNC_7F313_